#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include <PRinternal/osint.h>

s32 osSendMesg(OSMesgQueue* mq, OSMesg msg, s32 flags) {
    register u32 save_mask;
    register s32 last;

    if ((flags != OS_MESG_NOBLOCK) && (flags != OS_MESG_BLOCK)) {
        __osError(ERR_OSSENDMESG, 1, flags);
        return -1;
    }

    save_mask = __osDisableInt();

    while (MQ_IS_FULL(mq)) {
        if (flags == OS_MESG_BLOCK) {
            __osRunningThread->state = OS_STATE_WAITING;
            __osEnqueueAndYield(&mq->fullqueue);
        }
        else {
            __osRestoreInt(save_mask);
            return -1;
        }
    }

    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = msg;
    mq->validCount++;

    if (mq->mtqueue->next != NULL) {
        osStartThread(__osPopThread(&mq->mtqueue));
    }

    __osRestoreInt(save_mask);
    return 0;
}
