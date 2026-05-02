#include <PR/libaudio.h>
#include <PR/os.h>
#include <audio/initfx.h>
#include <audio/synthInternals.h>

#define SCALE (16384)
#define MS *(((s32)((f32)44.1f)) & ~0x7)

static s32 sSmallRoomParams[26] = {
    3,
    100 MS,
    0,
    54 MS,
    9830,
    -9830,
    0,
    0,
    0,
    0,
    19 MS,
    38 MS,
    3276,
    -3276,
    0x3FFF,
    0,
    0,
    0,
    0,
    60 MS,
    5000,
    0,
    0,
    0,
    0,
    0x5000,
};

static s32 sBigRoomParams[34] = {
    4,
    100 MS,
    0,
    66 MS,
    9830,
    -9830,
    0,
    0,
    0,
    0,
    22 MS,
    54 MS,
    3276,
    -3276,
    0x3FFF,
    0,
    0,
    0,
    66 MS,
    91 MS,
    3276,
    -3276,
    0x3FFF,
    0,
    0,
    0,
    0,
    94 MS,
    8000,
    0,
    0,
    0,
    0,
    0x5000,
};

static s32 sEchoParams[10] = {
    1,
    200 MS,
    0,
    179 MS,
    12000,
    0,
    0x7FFF,
    0,
    0,
    0,
};

static s32 sChorusParams[10] = {
    1,
    20 MS,
    0,
    5 MS,
    0x4000,
    0,
    0x7FFF,
    7600,
    700,
    0,
};

static s32 sFlangeParams[10] = {
    1,
    20 MS,
    0,
    5 MS,
    0,
    0x5FFF,
    0x7FFF,
    380,
    500,
    0,
};

static s32 sNullParams[10] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

extern Acmd* alFxPull(void* filter, s16* out, s32 out_count, s32 sample_offset, Acmd* cmd);
extern s32 alFxParam(void* filter, s32 param_id, void* param);
extern s32 alFxParamHdl(void* filter, s32 param_id, void* param);

void _init_lpfilter(ALLowPass* low_pass) {
    s32 index;
    s32 temp;
    s16 fc;
    f64 ffc;
    f64 fcoef;

    temp = low_pass->fc * SCALE;
    fc = temp >> 15;
    low_pass->fgain = SCALE - fc;
    low_pass->first = 1;

    for (index = 0; index < 8; index++) {
        low_pass->fcvec.fccoef[index] = 0;
    }

    low_pass->fcvec.fccoef[index++] = fc;
    fcoef = ffc = (f64)fc / SCALE;

    for (; index < 16; index++) {
        fcoef *= ffc;
        low_pass->fcvec.fccoef[index] = (s16)(fcoef * SCALE);
    }
}

void alFxNew(ALFx* fx, ALSynConfig* config, ALHeap* heap) {
    u16 index;
    u16 param_index;
    u16 section_index;
    s32* param = NULL;
    ALFilter* filter = (ALFilter*)fx;
    ALDelay* delay;

    alFilterNew(filter, NULL, alFxParam, AL_FX);
    filter->handler = alFxPull;
    fx->paramHdl = (ALSetFXParam)alFxParamHdl;

    switch (config->fxType) {
        case AL_FX_SMALLROOM:
            param = sSmallRoomParams;
            break;

        case AL_FX_BIGROOM:
            param = sBigRoomParams;
            break;

        case AL_FX_ECHO:
            param = sEchoParams;
            break;

        case AL_FX_CHORUS:
            param = sChorusParams;
            break;

        case AL_FX_FLANGE:
            param = sFlangeParams;
            break;

        case AL_FX_CUSTOM:
            param = config->params;
            break;

        default:
            param = sNullParams;
            break;
    }

    param_index = 0;
    fx->section_count = param[param_index++];
    fx->length = param[param_index++];

#line 143 "../audio/drvrNew.c"
    fx->delay = alHeapAlloc(heap, fx->section_count, sizeof(ALDelay));
    fx->base = alHeapAlloc(heap, fx->length, sizeof(s16));
#line 124
    fx->input = fx->base;

    for (section_index = 0; section_index < fx->length; section_index++) {
        fx->base[section_index] = 0;
    }

    for (index = 0; index < fx->section_count; index++) {
        delay = &fx->delay[index];
        delay->input = param[param_index++];
        delay->output = param[param_index++];
        delay->fbcoef = param[param_index++];
        delay->ffcoef = param[param_index++];
        delay->gain = param[param_index++];

        if (param[param_index]) {
            delay->rsinc = ((((f32)param[param_index++]) / 1000) * 2.0) / config->outputRate;
            delay->rsgain = (((f32)param[param_index++]) / 173123.404906676) * (delay->output - delay->input);
            delay->rsval = 1.0;
            delay->rsdelta = 0.0;
#line 180 "../audio/drvrNew.c"
            delay->rs = alHeapAlloc(heap, 1, sizeof(ALResampler));
            delay->rs->state = alHeapAlloc(heap, 1, sizeof(RESAMPLE_STATE));
#line 151
            delay->rs->delta = 0.0;
            delay->rs->first = 1;
        }
        else {
            delay->rs = NULL;
            param_index++;
            param_index++;
        }

        if (param[param_index]) {
#line 191 "../audio/drvrNew.c"
            delay->lp = alHeapAlloc(heap, 1, sizeof(ALLowPass));
            delay->lp->fstate = alHeapAlloc(heap, 1, sizeof(POLEF_STATE));
#line 164
            delay->lp->fc = param[param_index++];
            _init_lpfilter(delay->lp);
        }
        else {
            delay->lp = NULL;
            param_index++;
        }
    }
}

void alEnvmixerNew(ALEnvMixer* envmixer, ALHeap* heap) {
    alFilterNew((ALFilter*)envmixer, alEnvmixerPull, alEnvmixerParam, AL_ENVMIX);

#line 206 "../audio/drvrNew.c"
    envmixer->state = alHeapAlloc(heap, 1, sizeof(ENVMIX_STATE));
#line 181

    envmixer->first = 1;
    envmixer->motion = AL_STOPPED;
    envmixer->volume = 1;
    envmixer->ltgt = 1;
    envmixer->rtgt = 1;
    envmixer->cvolL = 1;
    envmixer->cvolR = 1;
    envmixer->dryamt = 0;
    envmixer->wetamt = 0;
    envmixer->lratm = 1;
    envmixer->lratl = 0;
    envmixer->lratm = 1;
    envmixer->lratl = 0;
    envmixer->delta = 0;
    envmixer->segEnd = 0;
    envmixer->pan = 0;
    envmixer->ctrlList = NULL;
    envmixer->ctrlTail = NULL;
    envmixer->sources = NULL;
}

void alLoadNew(ALLoadFilter* filter, ALDMANew dma_new, ALHeap* heap) {
    s32 i;

    alFilterNew((ALFilter*)filter, alAdpcmPull, alLoadParam, AL_ADPCM);

#line 240 "../audio/drvrNew.c"
    filter->state = alHeapAlloc(heap, 1, sizeof(ADPCM_STATE));
    filter->lstate = alHeapAlloc(heap, 1, sizeof(ADPCM_STATE));
#line 215

    filter->dma = dma_new(&filter->dmaState);
    filter->lastsam = 0;
    filter->first = 1;
    filter->memin = 0;
}

void alResampleNew(ALResampler* resampler, ALHeap* heap) {
    alFilterNew((ALFilter*)resampler, alResamplePull, alResampleParam, AL_RESAMPLE);

#line 260 "../audio/drvrNew.c"
    resampler->state = alHeapAlloc(heap, 1, sizeof(RESAMPLE_STATE));
#line 229
    resampler->delta = 0.0;
    resampler->first = 1;
    resampler->motion = AL_STOPPED;
    resampler->ratio = 1.0;
    resampler->upitch = 0;
    resampler->ctrlList = NULL;
    resampler->ctrlTail = NULL;
}

void alAuxBusNew(ALAuxBus* bus, void* sources, s32 max_sources) {
    alFilterNew((ALFilter*)bus, alAuxBusPull, alAuxBusParam, AL_AUXBUS);

    bus->sourceCount = 0;
    bus->maxSources = max_sources;
    bus->sources = (ALFilter**)sources;
}

void alMainBusNew(ALMainBus* bus, void* sources, s32 max_sources) {
    alFilterNew((ALFilter*)bus, alMainBusPull, alMainBusParam, AL_MAINBUS);

    bus->sourceCount = 0;
    bus->maxSources = max_sources;
    bus->sources = (ALFilter**)sources;
}

void alSaveNew(ALSave* save) {
    alFilterNew((ALFilter*)save, alSavePull, alSaveParam, AL_SAVE);

    save->dramout = NULL;
    save->first = 1;
}
