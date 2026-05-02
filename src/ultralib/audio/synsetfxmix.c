#include <audio/synthInternals.h>
#include <PR/libaudio.h>
#include "PR/os_internal.h"
#include "PR/ultraerror.h"

void alSynSetFXMix(ALSynth* synth, ALVoice* voice, u8 fx_mix) {
    ALParam* update;
    ALFilter* filter;

    if (voice->pvoice) {
        update = __allocParam();
        ALFailIf(update == NULL, ERR_ALSYN_NO_UPDATE);

        update->delta = synth->paramSamples + voice->pvoice->offset;
        update->type = AL_FILTER_SET_FXAMT;

        if (fx_mix < 0) {
            update->data.i = -fx_mix;
        }
        else {
            update->data.i = fx_mix;
        }

        update->next = NULL;

        filter = voice->pvoice->channelKnob;
        (*filter->setParam)(filter, AL_FILTER_ADD_UPDATE, update);
    }
}
