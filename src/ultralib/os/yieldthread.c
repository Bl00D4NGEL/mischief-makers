#include "PR/os_internal.h"
#include "osint.h"

void osYieldThread(void) {
    register u32 save_mask;

    save_mask = __osDisableInt();
    __osRunningThread->state = OS_STATE_RUNNABLE;
    __osEnqueueAndYield(&__osRunQueue);
    __osRestoreInt(save_mask);
}
