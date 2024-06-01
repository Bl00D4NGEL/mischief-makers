#include <macros.h>
#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include "osint.h"

__OSEventState __osEventStateTab[OS_NUM_EVENTS] ALIGNED(8);

void osSetEventMesg(OSEvent event, OSMesgQueue* mq, OSMesg msg) {
    register u32 saveMask;
    __OSEventState* es;

    if (event >= OS_NUM_EVENTS) {
        __osError(ERR_OSSETEVENTMESG, 1, event);
        return;
    }

    saveMask = __osDisableInt();

    es = &__osEventStateTab[event];

    es->messageQueue = mq;
    es->message = msg;

    __osRestoreInt(saveMask);
}

