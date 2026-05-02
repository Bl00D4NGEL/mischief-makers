#include <audio/synthInternals.h>
#include "PR/os_internal.h"
#include "PR/ultraerror.h"

void alSynSetVol(ALSynth* synth, ALVoice* voice, s16 volume, ALMicroTime time) {
    ALParam* update;
    ALFilter* filter;

    if (voice->pvoice) {
        update = __allocParam();
        ALFailIf(update == NULL, ERR_ALSYN_NO_UPDATE);

        update->delta = synth->paramSamples + voice->pvoice->offset;
        update->type = AL_FILTER_SET_VOLUME;
        update->data.i = volume;
        update->moredata.i = _timeToSamples(synth, time);
        update->next = NULL;

        filter = voice->pvoice->channelKnob;
        (*filter->setParam)(filter, AL_FILTER_ADD_UPDATE, update);
    }
}
