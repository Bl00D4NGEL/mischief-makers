#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include <PR/rcp.h>
#include "PRinternal/viint.h"

#define OS_VI_SPECIAL_MAX OS_VI_DITHER_FILTER_OFF

// Configures VI special-feature bits to be applied on the next context swap
// If the same setting is enabled and disabled in one call, disabled wins
void osViSetSpecialFeatures(u32 features) {
    register u32 save_mask;

    if (!__osViDevMgr.active) {
        __osError(ERR_OSVISETSPECIAL_VIMGR, 0);
        return;
    }

    if ((features < OS_VI_GAMMA_ON) || (features > OS_VI_SPECIAL_MAX)) {
        __osError(ERR_OSVISETSPECIAL_VALUE, 1, features);
        return;
    }

    save_mask = __osDisableInt();

    if ((features & OS_VI_GAMMA_ON) != 0) {
        __osViNext->control |= VI_CTRL_GAMMA_ON;
    }

    if ((features & OS_VI_GAMMA_OFF) != 0) {
        __osViNext->control &= ~VI_CTRL_GAMMA_ON;
    }

    if ((features & OS_VI_GAMMA_DITHER_ON) != 0) {
        __osViNext->control |= VI_CTRL_GAMMA_DITHER_ON;
    }

    if ((features & OS_VI_GAMMA_DITHER_OFF) != 0) {
        __osViNext->control &= ~VI_CTRL_GAMMA_DITHER_ON;
    }

    if ((features & OS_VI_DIVOT_ON) != 0) {
        __osViNext->control |= VI_CTRL_DIVOT_ON;
    }

    if ((features & OS_VI_DIVOT_OFF) != 0) {
        __osViNext->control &= ~VI_CTRL_DIVOT_ON;
    }

    if ((features & OS_VI_DITHER_FILTER_ON) != 0) {
        __osViNext->control |= VI_CTRL_DITHER_FILTER_ON;
        __osViNext->control &= ~VI_CTRL_ANTIALIAS_MASK;
    }

    if ((features & OS_VI_DITHER_FILTER_OFF) != 0) {
        __osViNext->control &= ~VI_CTRL_DITHER_FILTER_ON;
        __osViNext->control |= __osViNext->modep->comRegs.ctrl & VI_CTRL_ANTIALIAS_MASK;
    }

    __osViNext->state |= VI_STATE_CTRL_UPDATED;

    __osRestoreInt(save_mask);
}
