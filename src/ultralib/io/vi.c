#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/R4300.h"
#include "PR/rcp.h"
#include "viint.h"

static __OSViContext sViContexts[2] ALIGNED(0x8) = {0};
__OSViContext* __osViCurr = &sViContexts[0];
__OSViContext* __osViNext = &sViContexts[1];

void __osViInit(void) {
    bzero(sViContexts, sizeof(sViContexts));
    __osViCurr = &sViContexts[0];
    __osViNext = &sViContexts[1];
    __osViNext->retraceCount = 1;
    __osViCurr->retraceCount = 1;
    __osViNext->framep = (void*)K0BASE;
    __osViCurr->framep = (void*)K0BASE;

    if (osTvType == OS_TV_TYPE_PAL) {
        __osViNext->modep = &osViModePalLan1;
    }
    else if (osTvType == OS_TV_TYPE_MPAL) {
        __osViNext->modep = &osViModeMpalLan1;
    }
    else {
        __osViNext->modep = &osViModeNtscLan1;
    }

    __osViNext->state = VI_STATE_BLACK;
    __osViNext->control = __osViNext->modep->comRegs.ctrl;

    while (IO_READ(VI_CURRENT_REG) > 10) {
    }

    IO_WRITE(VI_CONTROL_REG, 0);
    __osViSwapContext();
}
