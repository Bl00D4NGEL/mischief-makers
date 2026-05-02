#include <audio/synthInternals.h>
#include "PR/os_internal.h"
#include "PR/ultraerror.h"

s32 _allocatePVoice(ALSynth* synth, PVoice** voice, s16 priority);

s32 alSynAllocVoice(ALSynth* synth, ALVoice* voice, ALVoiceConfig* config) {
    PVoice* pvoice = NULL;
    ALFilter* filter;
    ALParam* update;
    s32 stolen;

    if (synth->paramList == NULL) {
        __osError(ERR_ALSYN_NO_UPDATE, 0);
        return 0;
    }
    else if (synth->paramList->next == NULL) {
        __osError(ERR_ALSYN_NO_UPDATE, 0);
        return 0;
    }

    voice->priority = config->priority;
    voice->unityPitch = config->unityPitch;
    voice->table = NULL;
    voice->fxBus = config->fxBus;
    voice->state = AL_STOPPED;
    voice->pvoice = NULL;

    stolen = _allocatePVoice(synth, &pvoice, config->priority);

    if (pvoice) {
        filter = pvoice->channelKnob;

        if (stolen) {
            pvoice->offset = 512;
            pvoice->vvoice->pvoice = NULL;

            update = __allocParam();
            update->delta = synth->paramSamples;
            update->type = AL_FILTER_SET_VOLUME;
            update->data.i = 0;
            update->moredata.i = pvoice->offset - 64;
            (*filter->setParam)(filter, AL_FILTER_ADD_UPDATE, update);

            update = __allocParam();
            if (update) {
                update->delta = synth->paramSamples + pvoice->offset;
                update->type = AL_FILTER_STOP_VOICE;
                update->next = NULL;
                (*filter->setParam)(filter, AL_FILTER_ADD_UPDATE, update);
            }
            else {
                __osError(ERR_ALSYN_NO_UPDATE, 0);
            }
        }
        else {
            pvoice->offset = 0;
        }

        pvoice->vvoice = voice;
        voice->pvoice = pvoice;
    }

    return pvoice != NULL;
}

s32 _allocatePVoice(ALSynth* synth, PVoice** voice, s16 priority) {
    ALLink* link;
    PVoice* pvoice;
    s32 stolen = 0;

    if ((link = synth->pLameList.next) != NULL) {
        *voice = (PVoice*)link;
        alUnlink(link);
        alLink(link, &synth->pAllocList);
    }
    else if ((link = synth->pFreeList.next) != NULL) {
        *voice = (PVoice*)link;
        alUnlink(link);
        alLink(link, &synth->pAllocList);
    }
    else {
        for (link = synth->pAllocList.next; link != NULL; link = link->next) {
            pvoice = (PVoice*)link;

            if ((pvoice->vvoice->priority <= priority) && (pvoice->offset == 0)) {
                *voice = pvoice;
                priority = pvoice->vvoice->priority;
                stolen = 1;
            }
        }
    }

    return stolen;
}
