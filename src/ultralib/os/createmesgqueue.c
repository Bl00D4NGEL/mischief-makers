#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include "osint.h"

void osCreateMesgQueue(OSMesgQueue* mq, OSMesg* msg, s32 msg_count) {
    if (msg_count <= 0) {
        __osError(ERR_OSCREATEMESGQUEUE, 1, msg_count);
        return;
    }

    mq->mtqueue = (OSThread*)&__osThreadTail.next;
    mq->fullqueue = (OSThread*)&__osThreadTail.next;
    mq->validCount = 0;
    mq->first = 0;
    mq->msgCount = msg_count;
    mq->msg = msg;
}
