#include "PR/os_internal.h"

void __osSetGlobalIntMask(OSHWIntr mask) {
    register u32 save_mask;

    save_mask = __osDisableInt();
    __OSGlobalIntMask |= mask;
    __osRestoreInt(save_mask);
}
