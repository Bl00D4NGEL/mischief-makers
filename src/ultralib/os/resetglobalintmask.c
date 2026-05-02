#include "PR/os_internal.h"

void __osResetGlobalIntMask(OSHWIntr mask) {
    register u32 save_mask;

    save_mask = __osDisableInt();
    __OSGlobalIntMask &= ~(mask & ~OS_IM_RCP);
    __osRestoreInt(save_mask);
}
