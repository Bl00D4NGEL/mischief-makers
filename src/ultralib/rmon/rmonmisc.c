#include "PRinternal/dbgproto.h"
#include "PR/os_internal.h"
#include "PR/sptask.h"
#include "PRinternal/macros.h"

OSThread rmonIOThread ALIGNED(8);
OSMesg rmonMsgs[8] ALIGNED(8);
STACK(rmonIOStack, 0x4000)
ALIGNED(16);
OSMesg rmonPiMsgs[8] ALIGNED(8);
OSMesgQueue rmonPiMQ ALIGNED(8);
OSMesgQueue __rmonMQ ALIGNED(8);

#include "PRinternal/rmonint.h"

int __rmonSetFault(KKHeader* request_header) {
    KKFaultRequest* request = (KKFaultRequest*)request_header;
    KKObjectEvent reply;

    STUBBED_PRINTF(("SetFault\n"));

    reply.header.code = request->header.code;
    reply.header.error = TV_ERROR_NO_ERROR;
    reply.object = request->tid;

    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);
    return TV_ERROR_NO_ERROR;
}

void __rmonInit(void) {
    osCreateMesgQueue(&__rmonMQ, rmonMsgs, ARRLEN(rmonMsgs));
    osSetEventMesg(OS_EVENT_CPU_BREAK, &__rmonMQ, (OSMesg)RMON_MESG_CPU_BREAK);
    osSetEventMesg(OS_EVENT_SP_BREAK, &__rmonMQ, (OSMesg)RMON_MESG_SP_BREAK);
    osSetEventMesg(OS_EVENT_FAULT, &__rmonMQ, (OSMesg)RMON_MESG_FAULT);
    osSetEventMesg(OS_EVENT_THREADSTATUS, &__rmonMQ, NULL);
    osCreateThread(&rmonIOThread, 0, (void (*)(void*))__rmonIOhandler, NULL, STACK_START(rmonIOStack),
                   OS_PRIORITY_MAX);
    osCreatePiManager(OS_PRIORITY_PIMGR, &rmonPiMQ, rmonPiMsgs, ARRLEN(rmonPiMsgs));
    osStartThread(&rmonIOThread);
}

void __rmonPanic(void) {
    STUBBED_PRINTF(("PANIC!!\n"));

    for (;;) {
    }
}

int __rmonSetComm(KKHeader* request_header) {
    KKObjectEvent reply;

    STUBBED_PRINTF(("SetComm\n"));

    reply.header.code = request_header->code;
    reply.object = 0;
    reply.header.error = TV_ERROR_NO_ERROR;

    __rmonSendReply(&reply.header, sizeof(reply), KK_TYPE_REPLY);

    return TV_ERROR_NO_ERROR;
}
