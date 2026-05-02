#include <PR/libaudio.h>
#include <PR/os.h>
#include <audio/synthInternals.h>

Acmd* alResamplePull(void* filter, s16* outp, s32 outCnt, s32 sampleOffset, Acmd* p) {
    ALResampler* f = (ALResampler*)filter;
    Acmd* ptr = p;
    s16 inp;
    s32 inCount;
    ALFilter* source = f->filter.source;
    s32 incr;
    f32 finCount;

    inp = AL_DECODER_OUT;

    if (!outCnt) {
        return ptr;
    }

    if (f->upitch) {
        ptr = (*source->handler)(source, &inp, outCnt, sampleOffset, p);
        aDMEMMove(ptr++, inp, *outp, outCnt << 1);
    }
    else {
        if (f->ratio > MAX_RATIO) {
            f->ratio = MAX_RATIO;
        }

        f->ratio = (s32)(f->ratio * UNITY_PITCH);
        f->ratio = f->ratio / UNITY_PITCH;

        finCount = f->delta + (f->ratio * (f32)outCnt);
        inCount = (s32)finCount;
        f->delta = finCount - (f32)inCount;

        ptr = (*source->handler)(source, &inp, inCount, sampleOffset, p);

        incr = (s32)(f->ratio * UNITY_PITCH);
        aSetBuffer(ptr++, 0, inp, *outp, outCnt << 1);
        aResample(ptr++, f->first, incr, osVirtualToPhysical(f->state));
        f->first = 0;
    }

    return ptr;
}

s32 alResampleParam(void* filter, s32 paramID, void* param) {
    ALFilter* f = (ALFilter*)filter;
    ALResampler* r = (ALResampler*)filter;
    union {
        f32 f;
        s32 i;
    } data;

    switch (paramID) {
        case AL_FILTER_SET_SOURCE:
            f->source = (ALFilter*)param;
            break;

        case AL_FILTER_RESET:
            r->delta = 0.0;
            r->first = 1;
            r->motion = AL_STOPPED;
            r->upitch = 0;
            if (f->source) {
                (*f->source->setParam)(f->source, AL_FILTER_RESET, 0);
            }
            break;

        case AL_FILTER_START:
            r->motion = AL_PLAYING;
            if (f->source) {
                (*f->source->setParam)(f->source, AL_FILTER_START, 0);
            }
            break;

        case AL_FILTER_SET_PITCH:
            data.i = (s32)param;
            r->ratio = data.f;
            break;

        case AL_FILTER_SET_UNITY_PITCH:
            r->upitch = 1;
            break;

        default:
            if (f->source) {
                (*f->source->setParam)(f->source, paramID, param);
            }
            break;
    }

    return 0;
}
