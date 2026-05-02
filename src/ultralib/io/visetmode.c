#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "assert.h"
#include "PRinternal/viint.h"

void osViSetMode(OSViMode* modep) {
    register u32 save_mask;

    if (!__osViDevMgr.active) {
        __osError(ERR_OSVISETMODE, 0);
        return;
    }

    assert(modep != NULL, "../io/visetmode.c", 58);

    save_mask = __osDisableInt();

    __osViNext->modep = modep;
    __osViNext->state = VI_STATE_MODE_UPDATED;
    __osViNext->control = __osViNext->modep->comRegs.ctrl;
    __osRestoreInt(save_mask);
}
