#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include "assert.h"
#include "PRinternal/viint.h"

void osViSetEvent(OSMesgQueue* mq, OSMesg msg, u32 retrace_count) {
    register u32 save_mask;

    if (!__osViDevMgr.active) {
        __osError(ERR_OSVISETEVENT, 0);
        return;
    }

    assert(mq != NULL, "../io/visetevent.c", 61);

    save_mask = __osDisableInt();

    __osViNext->msgq = mq;
    __osViNext->msg = msg;
    __osViNext->retraceCount = retrace_count;
    __osRestoreInt(save_mask);
}
