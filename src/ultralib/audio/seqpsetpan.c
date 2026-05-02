#include <PR/libaudio.h>

void alSeqpSetChlPan(ALSeqPlayer* seqp, u8 chan, ALPan pan) {
    ALEvent event;

    event.type = AL_SEQP_MIDI_EVT;
    event.msg.midi.ticks = 0;
    event.msg.midi.status = AL_MIDI_ControlChange | chan;
    event.msg.midi.byte1 = AL_MIDI_PAN_CTRL;
    event.msg.midi.byte2 = pan;

    alEvtqPostEvent(&seqp->evtq, &event, 0);
}
