#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/viint.h"

void __osViSwapContext(void) {
    register OSViMode* vi_mode;
    register __OSViContext* vi_context;
    u32 origin;
    u32 h_start;
    u32 nom_value;
    u32 field;

    field = 0;
    vi_context = __osViNext;
    vi_mode = vi_context->modep;

    field = IO_READ(VI_CURRENT_REG) & 1;

    origin = osVirtualToPhysical(vi_context->framep) + vi_mode->fldRegs[field].origin;
    if (vi_context->state & VI_STATE_XSCALE_UPDATED) {
        vi_context->x.scale |= vi_mode->comRegs.xScale & ~VI_SCALE_MASK;
    }
    else {
        vi_context->x.scale = vi_mode->comRegs.xScale;
    }

    if (vi_context->state & VI_STATE_YSCALE_UPDATED) {
        nom_value = vi_mode->fldRegs[field].yScale & VI_SCALE_MASK;
        vi_context->y.scale = vi_context->y.factor * nom_value;
        vi_context->y.scale |= vi_mode->fldRegs[field].yScale & ~VI_SCALE_MASK;
    }
    else {
        vi_context->y.scale = vi_mode->fldRegs[field].yScale;
    }

    h_start = vi_mode->comRegs.hStart;

    if (vi_context->state & VI_STATE_BLACK) {
        h_start = 0;
    }

    if (vi_context->state & VI_STATE_REPEATLINE) {
        vi_context->y.scale = 0;
        origin = osVirtualToPhysical(vi_context->framep);
    }

    if (vi_context->state & VI_STATE_FADE) {
        vi_context->y.scale = (vi_context->y.offset << VI_SUBPIXEL_SH) & (VI_2_10_FPART_MASK << VI_SUBPIXEL_SH);
        origin = osVirtualToPhysical(vi_context->framep);
    }

    IO_WRITE(VI_ORIGIN_REG, origin);
    IO_WRITE(VI_WIDTH_REG, vi_mode->comRegs.width);
    IO_WRITE(VI_BURST_REG, vi_mode->comRegs.burst);
    IO_WRITE(VI_V_SYNC_REG, vi_mode->comRegs.vSync);
    IO_WRITE(VI_H_SYNC_REG, vi_mode->comRegs.hSync);
    IO_WRITE(VI_LEAP_REG, vi_mode->comRegs.leap);
    IO_WRITE(VI_H_START_REG, h_start);
    IO_WRITE(VI_V_START_REG, vi_mode->fldRegs[field].vStart);
    IO_WRITE(VI_V_BURST_REG, vi_mode->fldRegs[field].vBurst);
    IO_WRITE(VI_INTR_REG, vi_mode->fldRegs[field].vIntr);
    IO_WRITE(VI_X_SCALE_REG, vi_context->x.scale);
    IO_WRITE(VI_Y_SCALE_REG, vi_context->y.scale);
    IO_WRITE(VI_CONTROL_REG, vi_context->control);

    __osViNext = __osViCurr;
    __osViCurr = vi_context;
    *__osViNext = *__osViCurr;
}
