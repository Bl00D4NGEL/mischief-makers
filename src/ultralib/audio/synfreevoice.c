#include "PR/ultraerror.h"
#include "PR/os_internal.h"
#include <audio/synthInternals.h>

void alSynFreeVoice(ALSynth* drvr, ALVoice* voice) {
    ALFilter* filter;
    ALFreeParam* update;

    if (voice->pvoice) {
        if (voice->pvoice->offset) {
            update = (ALFreeParam*)__allocParam();
            ALFailIf(update == NULL, ERR_ALSYN_NO_UPDATE);

            update->delta = drvr->paramSamples + voice->pvoice->offset;
            update->type = AL_FILTER_FREE_VOICE;
            update->pvoice = voice->pvoice;

            filter = voice->pvoice->channelKnob;
            (*filter->setParam)(filter, AL_FILTER_ADD_UPDATE, update);
        }
        else {
            _freePVoice(drvr, voice->pvoice);
        }

        voice->pvoice = NULL;
    }
}
