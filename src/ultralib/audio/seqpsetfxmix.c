#include <PR/libaudio.h>

void alSeqpSetChlFXMix(ALSeqPlayer* seqp, u8 chan, u8 fxmix) {
    ALEvent event;

    event.type = AL_SEQP_MIDI_EVT;
    event.msg.midi.ticks = 0;
    event.msg.midi.status = AL_MIDI_ControlChange | chan;
    event.msg.midi.byte1 = AL_MIDI_FX1_CTRL;
    event.msg.midi.byte2 = fxmix;

    alEvtqPostEvent(&seqp->evtq, &event, 0);
}
