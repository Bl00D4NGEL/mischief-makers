#include <PR/libaudio.h>

void alSeqpSetChlVol(ALSeqPlayer* seqp, u8 chan, u8 vol) {
    ALEvent event;

    event.type = AL_SEQP_MIDI_EVT;
    event.msg.midi.ticks = 0;
    event.msg.midi.status = AL_MIDI_ControlChange | chan;
    event.msg.midi.byte1 = AL_MIDI_VOLUME_CTRL;
    event.msg.midi.byte2 = vol;

    alEvtqPostEvent(&seqp->evtq, &event, 0);
}
