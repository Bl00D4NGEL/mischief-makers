#include <PR/os_internal.h>
#include "viint.h"

void osViBlack(u8 active) {
    register u32 save_mask = __osDisableInt();

    if (active) {
        __osViNext->state |= VI_STATE_BLACK;
    }
    else {
        __osViNext->state &= ~VI_STATE_BLACK;
    }

    __osRestoreInt(save_mask);
}
