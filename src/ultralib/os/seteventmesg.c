#include <PRinternal/macros.h>
#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include "osint.h"

__OSEventState __osEventStateTab[OS_NUM_EVENTS] ALIGNED(0x8);

void osSetEventMesg(OSEvent event, OSMesgQueue* mq, OSMesg msg) {
    register u32 save_mask;
    __OSEventState* event_state;

    if (event >= OS_NUM_EVENTS) {
        __osError(ERR_OSSETEVENTMESG, 1, event);
        return;
    }

    save_mask = __osDisableInt();

    event_state = &__osEventStateTab[event];

    event_state->messageQueue = mq;
    event_state->message = msg;

    __osRestoreInt(save_mask);
}
