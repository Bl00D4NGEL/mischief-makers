#include <PR/libaudio.h>

void alSeqpPlay(ALSeqPlayer* seqp) {
    ALEvent event;

    event.type = AL_SEQP_PLAY_EVT;

    alEvtqPostEvent(&seqp->evtq, &event, 0);
}
