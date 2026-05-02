#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PRinternal/osint.h"
#include "PRinternal/viint.h"

int osSetTimer(OSTimer* timer, OSTime countdown, OSTime interval, OSMesgQueue* mq, OSMesg msg) {
    OSTime time;

    if (!__osViDevMgr.active) {
        __osError(ERR_OSSETTIMER, 0);
        return 0;
    }

    timer->next = NULL;
    timer->prev = NULL;
    timer->interval = interval;
    timer->value = (countdown != 0) ? countdown : interval;
    timer->mq = mq;
    timer->msg = msg;

    time = __osInsertTimer(timer);
    if (__osTimerList->next == timer) {
        __osSetTimerIntr(time);
    }

    return 0;
}
