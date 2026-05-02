#include <assert.h>
#include <PR/libaudio.h>
#include <PR/os.h>
#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include "audio/initfx.h"
#include "audio/synthInternals.h"

#define RANGE 2.0
#define CONVERT 173123.404906676

#define INPUT_PARAM 0
#define OUTPUT_PARAM 1
#define FBCOEF_PARAM 2
#define FFCOEF_PARAM 3
#define GAIN_PARAM 4
#define CHORUSRATE_PARAM 5
#define CHORUSDEPTH_PARAM 6
#define LPFILT_PARAM 7

#define SWAP(in, out)   \
    {                   \
        s16 temp = out; \
        out = in;       \
        in = temp;      \
    }

extern ALGlobals* alGlobals;

Acmd* _loadOutputBuffer(ALFx* fx, ALDelay* delay, s32 buffer, s32 input_count, Acmd* cmd);
Acmd* _loadBuffer(ALFx* fx, s16* current_ptr, s32 buffer, s32 count, Acmd* cmd);
Acmd* _saveBuffer(ALFx* fx, s16* current_ptr, s32 buffer, s32 count, Acmd* cmd);
Acmd* _filterBuffer(ALLowPass* low_pass, s32 buffer, s32 count, Acmd* cmd);
f32 _doModFunc(ALDelay* delay, s32 count);

static s32 sLengthIncrements[] = {L0_INC, L1_INC, L2_INC};

Acmd* alFxPull(void* filter, s16* outp, s32 outCount, s32 sampleOffset, Acmd* p) {
    Acmd* ptr = p;
    ALFx* r = (ALFx*)filter;
    ALFilter* source = r->filter.source;
    s16 i, buff1, buff2, input, output;
    s16* in_ptr;
    s16* out_ptr;
    s16 gain;
    s16* prev_out_ptr = 0;
    ALDelay* d;
    ALDelay* pd;

    assert(source, "../audio/reverb.c", 76);

    ptr = (*source->handler)(source, outp, outCount, sampleOffset, p);

    input = AL_AUX_L_OUT;
    output = AL_AUX_R_OUT;
    buff1 = AL_TEMP_0;
    buff2 = AL_TEMP_1;

    aSetBuffer(ptr++, 0, 0, 0, outCount << 1);
    aMix(ptr++, 0, 0xDA83, AL_AUX_L_OUT, input);
    aMix(ptr++, 0, 0x5A82, AL_AUX_R_OUT, input);
    ptr = _saveBuffer(r, r->input, input, outCount, ptr);

    aClearBuffer(ptr++, output, outCount << 1);

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];

        if (in_ptr == prev_out_ptr) {
            SWAP(buff1, buff2);
        }
        else {
            ptr = _loadBuffer(r, in_ptr, buff1, outCount, ptr);
        }

        ptr = _loadOutputBuffer(r, d, buff2, outCount, ptr);

        if (d->ffcoef) {
            aMix(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
            if (!d->rs && !d->lp) {
                ptr = _saveBuffer(r, out_ptr, buff2, outCount, ptr);
            }
        }

        if (d->fbcoef) {
            aMix(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
            ptr = _saveBuffer(r, in_ptr, buff1, outCount, ptr);
        }

        if (d->lp) {
            ptr = _filterBuffer(d->lp, buff2, outCount, ptr);
        }

        if (!d->rs) {
            ptr = _saveBuffer(r, out_ptr, buff2, outCount, ptr);
        }

        if (d->gain) {
            aMix(ptr++, 0, (u16)d->gain, buff2, output);
        }

        prev_out_ptr = &r->input[d->output];
    }

    r->input += outCount;
    if (r->input > &r->base[r->length]) {
        r->input -= r->length;
    }

    aDMEMMove(ptr++, output, AL_AUX_L_OUT, outCount << 1);

    return ptr;
}

s32 alFxParam(void* filter, s32 param_id, void* param) {
    if (param_id == AL_FILTER_SET_SOURCE) {
        ALFilter* fx_filter = (ALFilter*)filter;
        fx_filter->source = (ALFilter*)param;
    }

    return 0;
}

s32 alFxParamHdl(void* filter, s32 param_id, void* param) {
    ALFx* fx = (ALFx*)filter;
    s32 param_slot = (param_id - 2) % 8;
    s32 section = (param_id - 2) / 8;
    s32 value = *(s32*)param;

    switch (param_slot) {
        case INPUT_PARAM:
            fx->delay[section].input = (u32)value & 0xFFFFFFF8;
            break;

        case OUTPUT_PARAM:
            fx->delay[section].output = (u32)value & 0xFFFFFFF8;
            break;

        case FFCOEF_PARAM:
            fx->delay[section].ffcoef = (s16)value;
            break;

        case FBCOEF_PARAM:
            fx->delay[section].fbcoef = (s16)value;
            break;

        case GAIN_PARAM:
            fx->delay[section].gain = (s16)value;
            break;

        case CHORUSRATE_PARAM:
            fx->delay[section].rsinc = ((((f32)value) / 1000) * RANGE) / alGlobals->drvr.outputRate;
            break;

        case CHORUSDEPTH_PARAM:
            fx->delay[section].rsgain = (((f32)value) / CONVERT) *
                                        (fx->delay[section].output - fx->delay[section].input);
            break;

        case LPFILT_PARAM:
            if (fx->delay[section].lp) {
                fx->delay[section].lp->fc = (s16)value;
                _init_lpfilter(fx->delay[section].lp);
            }
            break;
    }

    return 0;
}

Acmd* _loadOutputBuffer(ALFx* fx, ALDelay* delay, s32 buffer, s32 input_count, Acmd* cmd) {
    Acmd* ptr = cmd;
    s32 ratio;
    s32 count;
    s32 resample_buffer = AL_TEMP_2;
    s16* output_ptr;
    f32 float_input_count;
    f32 float_ratio;
    f32 delta;
    s32 ram_align = 0;
    s32 length;
    static f32 value = 0.0;
    static f32 last_value = -10.0;
    static f32 blob = 0;

    if (delay->rs) {
        length = delay->output - delay->input;
        delta = _doModFunc(delay, input_count);
        delta /= length;
        delta = (s32)(delta * UNITY_PITCH);
        delta = delta / UNITY_PITCH;
        float_ratio = 1.0 - delta;

        float_input_count = delay->rs->delta + (float_ratio * (f32)input_count);
        count = (s32)float_input_count;
        delay->rs->delta = float_input_count - (f32)count;

        output_ptr = &fx->input[-(delay->output - delay->rsdelta)];
        ram_align = ((s32)output_ptr & 0x7) >> 1;
        ptr = _loadBuffer(fx, output_ptr - ram_align, resample_buffer, count + ram_align, ptr);

        ratio = (s32)(float_ratio * UNITY_PITCH);
        aSetBuffer(ptr++, 0, resample_buffer + (ram_align << 1), buffer, input_count << 1);
        aResample(ptr++, delay->rs->first, ratio, osVirtualToPhysical(delay->rs->state));

        delay->rs->first = 0;
        delay->rsdelta += count - input_count;
    }
    else {
        output_ptr = &fx->input[-delay->output];
        ptr = _loadBuffer(fx, output_ptr, buffer, input_count, ptr);
    }

    return ptr;
}

Acmd* _loadBuffer(ALFx* fx, s16* current_ptr, s32 buffer, s32 count, Acmd* cmd) {
    Acmd* ptr = cmd;
    s32 after_end;
    s32 before_end;
    s16* updated_ptr;
    s16* delay_end;

    delay_end = &fx->base[fx->length];

    if (current_ptr > delay_end) {
        __osError(ERR_ALMODDELAYOVERFLOW, 1, delay_end - current_ptr);
    }

    if (current_ptr < fx->base) {
        current_ptr += fx->length;
    }

    updated_ptr = current_ptr + count;

    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - current_ptr;

        aSetBuffer(ptr++, 0, buffer, 0, before_end << 1);
        aLoadBuffer(ptr++, osVirtualToPhysical(current_ptr));
        aSetBuffer(ptr++, 0, buffer + (before_end << 1), 0, after_end << 1);
        aLoadBuffer(ptr++, osVirtualToPhysical(fx->base));
    }
    else {
        aSetBuffer(ptr++, 0, buffer, 0, count << 1);
        aLoadBuffer(ptr++, osVirtualToPhysical(current_ptr));
    }

    aSetBuffer(ptr++, 0, 0, 0, count << 1);

    return ptr;
}

Acmd* _saveBuffer(ALFx* fx, s16* current_ptr, s32 buffer, s32 count, Acmd* cmd) {
    Acmd* ptr = cmd;
    s32 after_end;
    s32 before_end;
    s16* updated_ptr;
    s16* delay_end;

    delay_end = &fx->base[fx->length];

    if (current_ptr < fx->base) {
        current_ptr += fx->length;
    }

    updated_ptr = current_ptr + count;

    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - current_ptr;

        aSetBuffer(ptr++, 0, 0, buffer, before_end << 1);
        aSaveBuffer(ptr++, osVirtualToPhysical(current_ptr));
        aSetBuffer(ptr++, 0, 0, buffer + (before_end << 1), after_end << 1);
        aSaveBuffer(ptr++, osVirtualToPhysical(fx->base));
        aSetBuffer(ptr++, 0, 0, 0, count << 1);
    }
    else {
        aSetBuffer(ptr++, 0, 0, buffer, count << 1);
        aSaveBuffer(ptr++, osVirtualToPhysical(current_ptr));
    }

    return ptr;
}

Acmd* _filterBuffer(ALLowPass* low_pass, s32 buffer, s32 count, Acmd* cmd) {
    Acmd* ptr = cmd;

    aSetBuffer(ptr++, 0, buffer, buffer, count << 1);
    aLoadADPCM(ptr++, 32, osVirtualToPhysical(low_pass->fcvec.fccoef));
    aPoleFilter(ptr++, low_pass->first, low_pass->fgain, osVirtualToPhysical(low_pass->fstate));
    low_pass->first = 0;

    return ptr;
}

f32 _doModFunc(ALDelay* delay, s32 count) {
    f32 value;

    delay->rsval += delay->rsinc * count;
    delay->rsval = (delay->rsval > RANGE) ? delay->rsval - (RANGE * 2) : delay->rsval;

    value = delay->rsval;
    value = (value < 0) ? -value : value;
    value -= RANGE / 2;

    return delay->rsgain * value;
}
