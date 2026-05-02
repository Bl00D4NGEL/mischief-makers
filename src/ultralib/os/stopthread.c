#include "PR/os_internal.h"
#include "osint.h"

void osStopThread(OSThread* thread) {
    register u32 save_mask;
    register OSThread* previous_thread;
    register OSThread* current_thread;

    save_mask = __osDisableInt();
    if (thread == NULL) {
        thread = __osRunningThread;
    }
    else if (thread->state != OS_STATE_STOPPED) {
        __osDequeueThread(thread->queue, thread);
    }

    if (__osActiveQueue == thread) {
        __osActiveQueue = __osActiveQueue->tlnext;
    }
    else {
        previous_thread = __osActiveQueue;
        current_thread = previous_thread->tlnext;
        while (current_thread != NULL) {
            if (current_thread == thread) {
                previous_thread->tlnext = thread->tlnext;
                break;
            }

            previous_thread = current_thread;
            current_thread = previous_thread->tlnext;
        }
    }

    if (thread == __osRunningThread) {
        __osDispatchThread();
    }

    __osRestoreInt(save_mask);
}
