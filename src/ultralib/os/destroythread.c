#include <PR/os_internal.h>
#include <PRinternal/osint.h>

void osDestroyThread(OSThread* thread) {
    register u32 save_mask;
    register u16 state;

    save_mask = __osDisableInt();

    if (thread == NULL) {
        state = OS_STATE_RUNNING;
    }
    else {
        state = thread->state;
    }

    switch (state) {
        case OS_STATE_RUNNING:
            __osRunningThread->state = OS_STATE_STOPPED;
            __osEnqueueAndYield(NULL);
            break;

        case OS_STATE_RUNNABLE:
        case OS_STATE_WAITING:
            thread->state = OS_STATE_STOPPED;
            __osDequeueThread(thread->queue, thread);
            break;
    }

    __osRestoreInt(save_mask);
}
