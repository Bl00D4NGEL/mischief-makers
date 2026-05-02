#line 1 "../audio/synthesizer.c"
#include <audio/synthInternals.h>
extern void __assert(const char*, const char*, int);
#define assert(EX) ((EX) ? ((void)0) : __assert(#EX, __FILE__, __LINE__))
#line 20 "../audio/synthesizer.c"
#include <PR/os_version.h>

static s32 __nextSampleTime(ALSynth* drvr, ALPlayer** client);
static s32 _timeToSamplesNoRound(ALSynth* ALSynth, s32 micros);

// Public synthesis driver interfaces
void alSynNew(ALSynth* drvr, ALSynConfig* c) {
    s32 i;
    ALVoice* vv;
    PVoice* pv;
    ALVoice* vvoices;
    PVoice* pvoices;
    ALHeap* hp = c->heap;
    ALSave* save;
    ALFilter* sources;
    ALParam* params;
    ALParam* paramPtr;

    drvr->head = NULL;
    drvr->numPVoices = c->maxPVoices;
    drvr->curSamples = 0;
    drvr->paramSamples = 0;
    drvr->outputRate = c->outputRate;
    drvr->maxOutSamples = AL_MAX_RSP_SAMPLES;
    drvr->dma = (ALDMANew)c->dmaproc;

#line 61
    save = alHeapAlloc(hp, 1, sizeof(ALSave));
    alSaveNew(save);
    drvr->outputFilter = (ALFilter*)save;

    // Allocate and initialize the single auxiliary effects bus
#line 69
    drvr->auxBus = alHeapAlloc(hp, 1, sizeof(ALAuxBus));
    drvr->maxAuxBusses = 1;
#line 71
    sources = alHeapAlloc(hp, c->maxPVoices, sizeof(ALFilter*));
    alAuxBusNew(drvr->auxBus, sources, c->maxPVoices);

    // Allocate and initialize the main bus
#line 77
    drvr->mainBus = alHeapAlloc(hp, 1, sizeof(ALMainBus));
#line 78
    sources = alHeapAlloc(hp, c->maxPVoices, sizeof(ALFilter*));
    alMainBusNew(drvr->mainBus, sources, c->maxPVoices);

    if (c->fxType != AL_FX_NONE) {
        // Allocate an effect and set its parameters
        alSynAllocFX(drvr, 0, c, hp);
    }
    else {
        // Connect the auxiliary bus to the main bus
        alMainBusParam(drvr->mainBus, AL_FILTER_ADD_SOURCE, &drvr->auxBus[0]);
    }

    // Build the physical voice lists
    drvr->pFreeList.next = 0;
    drvr->pFreeList.prev = 0;
    drvr->pLameList.next = 0;
    drvr->pLameList.prev = 0;
    drvr->pAllocList.next = 0;
    drvr->pAllocList.prev = 0;

#line 102
    pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(PVoice));
    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];
        alLink((ALLink*)pv, &drvr->pFreeList);
        pv->vvoice = 0;

        alLoadNew(&pv->decoder, drvr->dma, hp);
        alLoadParam(&pv->decoder, AL_FILTER_SET_SOURCE, 0);

        alResampleNew(&pv->resampler, hp);
        alResampleParam(&pv->resampler, AL_FILTER_SET_SOURCE, &pv->decoder);

        alEnvmixerNew(&pv->envmixer, hp);
        alEnvmixerParam(&pv->envmixer, AL_FILTER_SET_SOURCE, &pv->resampler);

        alAuxBusParam(drvr->auxBus, AL_FILTER_ADD_SOURCE, &pv->envmixer);

        pv->channelKnob = (ALFilter*)&pv->envmixer;
    }

    alSaveParam(save, AL_FILTER_SET_SOURCE, drvr->mainBus);

    // Build the parameter update list.
#line 127
    params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
    drvr->paramList = 0;
    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = &params[i];
        paramPtr->next = drvr->paramList;
        drvr->paramList = paramPtr;
    }

    drvr->heap = hp;
}

// alAudioFrame runs every video frame using the video interrupt as the client time source
Acmd* alAudioFrame(Acmd* cmdList, s32* cmdLen, s16* outBuf, s32 outLen) {
    ALPlayer* client;
    ALFilter* output;
    ALSynth* drvr = &alGlobals->drvr;
    s16 tmp = 0; // Starting buffer in DMEM
    Acmd* cmdlEnd = cmdList;
    Acmd* cmdPtr;
    s32 nOut;
    s16* lOutBuf = outBuf;

    if (drvr->head == 0) {
        *cmdLen = 0;
        return cmdList; // Nothing to do.
    }

    // Run the client list and execute callbacks needed for this subframe
    // Work for the frame is scheduled up front, so offsets before the next frame execute early

    // paramSamples is the next parameter change time and curSamples is the current sample time
    // If the next change lands within this frame, call back the owning client
    // paramSamples must be rounded down to a 16-sample boundary for use by the client handler

    for (drvr->paramSamples = __nextSampleTime(drvr, &client);
         drvr->paramSamples - drvr->curSamples < outLen;
         drvr->paramSamples = __nextSampleTime(drvr, &client)) {
        drvr->paramSamples &= ~0xf;
        client->samplesLeft += _timeToSamplesNoRound(drvr, (*client->handler)(client));
    }

    // Keep paramSamples aligned to a 16-sample boundary
    // If a voice handler runs outside the ALVoiceHandler path, it gets an aligned timestamp and is processed during the next audio frame
    drvr->paramSamples &= ~0xf;

    // Build the command list in small chunks
    while (outLen > 0) {
        nOut = MIN(drvr->maxOutSamples, outLen);

        // Construct each physical voice command list by calling the head of the filter chain
        cmdPtr = cmdlEnd;
        aSegment(cmdPtr++, 0, 0);
        output = drvr->outputFilter;
        (*output->setParam)(output, AL_FILTER_SET_DRAM, lOutBuf);
        cmdlEnd = (*output->handler)(output, &tmp, nOut, drvr->curSamples,
                                     cmdPtr);

        outLen -= nOut;
        lOutBuf += nOut << 1; // For stereo
        drvr->curSamples += nOut;
    }
    *cmdLen = (s32)(cmdlEnd - cmdList);

    _collectPVoices(drvr);

    return cmdlEnd;
}

// Private synthesis driver interfaces

ALParam* __allocParam() {
    ALParam* update = 0;
    ALSynth* drvr = &alGlobals->drvr;

    if (drvr->paramList) {
        update = drvr->paramList;
        drvr->paramList = drvr->paramList->next;
        update->next = 0;
    }
    return update;
}

void __freeParam(ALParam* param) {
    ALSynth* drvr = &alGlobals->drvr;
    param->next = drvr->paramList;
    drvr->paramList = param;
}

void _collectPVoices(ALSynth* drvr) {
    ALLink* dl;
    PVoice* pv;

    while ((dl = drvr->pLameList.next) != 0) {
        pv = (PVoice*)dl;

        alUnlink(dl);
        alLink(dl, &drvr->pFreeList);
    }
}

void _freePVoice(ALSynth* drvr, PVoice* pvoice) {
    // Move the voice from the allocated list to the lame list
    alUnlink((ALLink*)pvoice);
    alLink((ALLink*)pvoice, &drvr->pLameList);
}

// Add 0.5 to offset the average truncation error from casting a float to int
s32 _timeToSamplesNoRound(ALSynth* synth, s32 micros) {
    f32 tmp = ((f32)micros) * synth->outputRate / 1000000.0 + 0.5;

    return (s32)tmp;
}

s32 _timeToSamples(ALSynth* synth, s32 micros) {
    return _timeToSamplesNoRound(synth, micros) & ~0xf;
}

static s32 __nextSampleTime(ALSynth* drvr, ALPlayer** client) {
    ALMicroTime delta = 0x7fffffff; // Max delta for s32
    ALPlayer* cl;
#line 307
    assert(drvr->head);

    *client = 0;

    for (cl = drvr->head; cl != 0; cl = cl->next) {
        if ((cl->samplesLeft - drvr->curSamples) < delta) {
            *client = cl;
            delta = cl->samplesLeft - drvr->curSamples;
        }
    }

    return (*client)->samplesLeft;
}
