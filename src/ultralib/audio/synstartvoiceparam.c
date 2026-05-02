#include <audio/synthInternals.h>
#include <PR/libaudio.h>
#include "PR/os_internal.h"
#include "PR/ultraerror.h"

void alSynStartVoiceParams(ALSynth* synth, ALVoice* voice, ALWaveTable* wavetable, f32 pitch, s16 volume, ALPan pan, u8 fx_mix, ALMicroTime delta) {
    ALStartParamAlt* update;
    ALFilter* filter;

    if (voice->pvoice) {
        update = (ALStartParamAlt*)__allocParam();
        ALFailIf(update == NULL, ERR_ALSYN_NO_UPDATE);

        if (fx_mix < 0) {
            fx_mix = -fx_mix;
        }

        update->delta = synth->paramSamples + voice->pvoice->offset;
        update->next = NULL;
        update->type = AL_FILTER_START_VOICE_ALT;
        update->unity = voice->unityPitch;
        update->pan = pan;
        update->volume = volume;
        update->fxMix = fx_mix;
        update->pitch = pitch;
        update->samples = _timeToSamples(synth, delta);
        update->wave = wavetable;

        filter = voice->pvoice->channelKnob;
        (*filter->setParam)(filter, AL_FILTER_ADD_UPDATE, update);
    }
}
