#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PR/rcp.h"
#include "PRinternal/viint.h"
#include "PRinternal/osint.h"

// data
OSDevMgr __osViDevMgr = {0};

// bss
static OSThread viThread;
static unsigned char viThreadStack[OS_VIM_STACKSIZE] ALIGNED(16);
static OSMesgQueue viEventQueue ALIGNED(8);
static OSMesg viEventBuf[5] ALIGNED(8);
static OSIoMesg viRetraceMsg ALIGNED(8);
static OSIoMesg viCounterMsg ALIGNED(8);
static u16 retrace; // required to OK?

static void viMgrMain(void* arg);

void osCreateViManager(OSPri pri) {
    u32 saved_mask;
    OSPri old_priority;
    OSPri my_priority;

    if ((pri < OS_PRIORITY_IDLE) || (pri > OS_PRIORITY_MAX)) {
        __osError(ERR_OSCREATEVIMANAGER, 1, pri);
        return;
    }

    if (__osViDevMgr.active) {
        return;
    }
    __osTimerServicesInit();

    osCreateMesgQueue(&viEventQueue, viEventBuf, ARRLEN(viEventBuf));
    viRetraceMsg.hdr.type = OS_MESG_TYPE_VRETRACE;
    viRetraceMsg.hdr.pri = OS_MESG_PRI_NORMAL;
    viRetraceMsg.hdr.retQueue = NULL;
    viCounterMsg.hdr.type = OS_MESG_TYPE_COUNTER;
    viCounterMsg.hdr.pri = OS_MESG_PRI_NORMAL;
    viCounterMsg.hdr.retQueue = NULL;
    osSetEventMesg(OS_EVENT_VI, &viEventQueue, &viRetraceMsg);
    osSetEventMesg(OS_EVENT_COUNTER, &viEventQueue, &viCounterMsg);
    old_priority = -1;
    my_priority = osGetThreadPri(NULL);

    if (my_priority < pri) {
        old_priority = my_priority;
        osSetThreadPri(NULL, pri);
    }

    saved_mask = __osDisableInt();
    __osViDevMgr.active = TRUE;
    __osViDevMgr.thread = &viThread;
    __osViDevMgr.cmdQueue = &viEventQueue;
    __osViDevMgr.evtQueue = &viEventQueue;
    __osViDevMgr.acsQueue = NULL;
    __osViDevMgr.dma = NULL;
    __osViDevMgr.edma = NULL;
    osCreateThread(&viThread, 0, viMgrMain, &__osViDevMgr, &viThreadStack[OS_VIM_STACKSIZE], pri);
    __osViInit();
    osStartThread(&viThread);
    __osRestoreInt(saved_mask);

    if (old_priority != -1) {
        osSetThreadPri(NULL, old_priority);
    }
}

static void viMgrMain(void* arg) {
    __OSViContext* vi_context;
    OSDevMgr* dev_mgr;
    OSIoMesg* message;
    s32 first;
    u32 count;

    message = NULL;
    first = 0;
    vi_context = __osViGetCurrentContext();
    retrace = vi_context->retraceCount;
    if (retrace == 0) {
        retrace = 1;
    }
    dev_mgr = (OSDevMgr*)arg;

    while (TRUE) {
        osRecvMesg(dev_mgr->evtQueue, (OSMesg)&message, OS_MESG_BLOCK);
        switch (message->hdr.type) {
            case OS_MESG_TYPE_VRETRACE:
                __osViSwapContext();
                retrace--;

                if (retrace == 0) {
                    vi_context = __osViGetCurrentContext();
                    if (vi_context->msgq != NULL) {
                        osSendMesg(vi_context->msgq, vi_context->msg, OS_MESG_NOBLOCK);
                    }
                    retrace = vi_context->retraceCount;
                }

                __osViIntrCount++;

                if (first) {
                    count = osGetCount();
                    __osCurrentTime = count;
                    first = 0;
                }

                count = __osBaseCounter;
                __osBaseCounter = osGetCount();
                count = __osBaseCounter - count;
                __osCurrentTime = __osCurrentTime + count;
                break;
            case OS_MESG_TYPE_COUNTER:
                __osTimerInterrupt();
                break;
            default:
                break;
        }
    }
}
