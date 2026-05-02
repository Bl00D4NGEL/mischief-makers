#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PR/ultralog.h"
#include "PR/sptask.h"
#include "PRinternal/dbgproto.h"
#include "PR/ramrom.h"
#include "PR/rdb.h"
#include "PR/rmon.h"
#include "PRinternal/macros.h"

static OSMesgQueue IOmq ALIGNED(8);
static OSMesg IOmsgs;
void* __osRdb_DbgRead_Buf;
u8 rmonRdbReadBuf[RMON_DBG_BUF_SIZE] ALIGNED(16);

#include "PRinternal/rmonint.h"

void __rmonSendFault(OSThread* thread) {
    volatile float f UNUSED;
    u8* thread_bytes;
    u32 sent = 0;

    // Touch the FPU so its registers are saved to the context structure
    f = 0.0f;

    thread_bytes = (u8*)thread;
    while (sent < sizeof(OSThread)) {
        sent += __osRdbSend(thread_bytes + sent, sizeof(OSThread) - sent, RDB_TYPE_GtoH_FAULT);
    }
}

void __rmonIOflush(void) {
    s32 sent = 0;
    char buffer[4];

    while (sent <= 0) {
        sent += __osRdbSend(buffer, 1, RDB_TYPE_GtoH_DEBUG_DONE);
    }
}

void __rmonIOputw(u32 word) {
    s32 sent = 0;
    char* word_bytes = (char*)&word;

    while (sent < 4) {
        sent += __osRdbSend(word_bytes + sent, sizeof(word) - sent, RDB_TYPE_GtoH_DEBUG);
    }
}

void __rmonIOhandler(void) {
    s32 sent;
    char buffer[4];

    osCreateMesgQueue(&IOmq, &IOmsgs, 1);
    osSetEventMesg(OS_EVENT_RDB_DBG_DONE, &IOmq, NULL);
    __osRdb_DbgRead_Buf = rmonRdbReadBuf;

    while (TRUE) {
        osRecvMesg(&IOmq, NULL, OS_MESG_BLOCK);

        __rmonExecute((KKHeader*)&rmonRdbReadBuf);
        __osRdb_DbgRead_Buf = rmonRdbReadBuf;

        sent = 0;
        while (sent <= 0) {
            sent += __osRdbSend(buffer, 1, RDB_TYPE_GtoH_DEBUG_READY);
        }
    }
}
