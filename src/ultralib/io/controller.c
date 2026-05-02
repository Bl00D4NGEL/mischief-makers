#include <PR/os_internal.h>
#include "controller.h"
#include "siint.h"

#define OS_USEC_TO_CYCLES_RUNTIME(usec) (((u64)(usec) * osClockRate) / 1000000)

OSPifRam __osContPifRam;
u8 __osContLastCmd;
u8 __osMaxControllers;
OSTimer __osEepromTimer;
OSMesgQueue __osEepromTimerQ ALIGNED(0x8);
OSMesg __osEepromTimerMsg;

s32 __osContinitialized = FALSE;
static u32 sControllerDataPad[3] = {0, 0, 0};

s32 osContInit(OSMesgQueue* mq, u8* bitpattern, OSContStatus* data) {
    OSMesg dummy;
    s32 ret = 0;
    OSTime time;
    OSTimer timer;
    OSMesgQueue timer_queue;

    if (__osContinitialized) {
        return 0;
    }

    __osContinitialized = TRUE;

    time = osGetTime();
    if (time < OS_USEC_TO_CYCLES_RUNTIME(500000)) {
        osCreateMesgQueue(&timer_queue, &dummy, 1);
        osSetTimer(&timer, OS_USEC_TO_CYCLES_RUNTIME(500000) - time, 0, &timer_queue, &dummy);
        osRecvMesg(&timer_queue, &dummy, OS_MESG_BLOCK);
    }

    __osMaxControllers = 4;

    __osPackRequestData(CONT_CMD_REQUEST_STATUS);

    ret = __osSiRawStartDma(OS_WRITE, __osContPifRam.ramarray);
    osRecvMesg(mq, &dummy, OS_MESG_BLOCK);

    ret = __osSiRawStartDma(OS_READ, __osContPifRam.ramarray);
    osRecvMesg(mq, &dummy, OS_MESG_BLOCK);

    __osContGetInitData(bitpattern, data);
    __osContLastCmd = CONT_CMD_REQUEST_STATUS;
    __osSiCreateAccessQueue();
    osCreateMesgQueue(&__osEepromTimerQ, &__osEepromTimerMsg, 1);

    return ret;
}

void __osContGetInitData(u8* pattern, OSContStatus* data) {
    u8* ptr;
    __OSContRequesFormat request_format;
    s32 index;
    u8 bits = 0;

    ptr = (u8*)__osContPifRam.ramarray;
    for (index = 0; index < __osMaxControllers; index++, ptr += sizeof(request_format), data++) {
        request_format = *(__OSContRequesFormat*)ptr;
        data->errno = CHNL_ERR(request_format);

        if (data->errno != 0) {
            continue;
        }

        data->type = request_format.typel << 8 | request_format.typeh;
        data->status = request_format.status;
        bits |= 1 << index;
    }

    *pattern = bits;
}

void __osPackRequestData(u8 cmd) {
    u8* ptr;
    __OSContRequesFormat request_format;
    s32 index;

    for (index = 0; index < ARRLEN(__osContPifRam.raw); index++) {
        __osContPifRam.ramarray[index] = 0;
    }

    __osContPifRam.pifstatus = CONT_CMD_EXE;
    ptr = (u8*)__osContPifRam.ramarray;
    request_format.dummy = CONT_CMD_NOP;
    request_format.txsize = CONT_CMD_RESET_TX;
    request_format.rxsize = CONT_CMD_RESET_RX;
    request_format.cmd = cmd;
    request_format.typeh = CONT_CMD_NOP;
    request_format.typel = CONT_CMD_NOP;
    request_format.status = CONT_CMD_NOP;
    request_format.dummy1 = CONT_CMD_NOP;

    for (index = 0; index < __osMaxControllers; index++) {
        *(__OSContRequesFormat*)ptr = request_format;
        ptr += sizeof(request_format);
    }

    *ptr = CONT_CMD_END;
}
