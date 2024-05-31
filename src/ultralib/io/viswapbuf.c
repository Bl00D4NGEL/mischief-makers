#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include <assert.h>
#include "viint.h"

void osViSwapBuffer(void* frameBufPtr) {
    u32 saveMask;
    if (!__osViDevMgr.active) {
        __osError(ERR_OSVISWAPBUFFER_VIMGR, 0);
        return;
    }

    assert(frameBufPtr != NULL, "../io/viswapbuf.c", 62);

    if ((u32)frameBufPtr & 0x3f) {
        __osError(ERR_OSVISWAPBUFFER_ADDR, 1, frameBufPtr);
        return;
    }

    saveMask = __osDisableInt();

    __osViNext->framep = frameBufPtr;
    __osViNext->state |= VI_STATE_BUFFER_UPDATED;
    __osRestoreInt(saveMask);
}

