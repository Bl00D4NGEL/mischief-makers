#include <PR/os_internal.h>
#include <PR/R4300.h>
#include <PR/ultraerror.h>
#include "osint.h"

void osCreateThread(OSThread* t, OSId id, void (*entry)(void*), void* arg, void* sp, OSPri p) {
    register u32 saveMask;
    OSIntMask mask;

    if ((u32)sp & 0x7) {
        __osError(ERR_OSCREATETHREAD_SP, 1, sp);
        return;
    }

    if ((p < OS_PRIORITY_IDLE) || (p > OS_PRIORITY_MAX)) {
        __osError(ERR_OSCREATETHREAD_PRI, 1, p);
        return;
    }

    t->id = id;
    t->priority = p;
    t->next = NULL;
    t->queue = NULL;
    t->context.pc = (u32)entry;
    t->context.a0 = (s64)(s32)arg; // Double cast gets rid of compiler warning
    t->context.sp = (s64)(s32)sp - 16;
    t->context.ra = (u64)__osCleanupThread;
    mask = OS_IM_ALL;
    t->context.sr = (mask & (SR_IMASK | SR_IE)) | SR_EXL;
    t->context.rcp = (mask & RCP_IMASK) >> RCP_IMASKSHIFT;
    t->context.fpcsr = (u32)(FPCSR_FS | FPCSR_EV);
    t->fp = 0;
    t->state = OS_STATE_STOPPED;
    t->flags = 0;

    saveMask = __osDisableInt();
    t->tlnext = __osActiveQueue;
    __osActiveQueue = t;
    __osRestoreInt(saveMask);
}

