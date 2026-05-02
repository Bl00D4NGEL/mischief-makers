#include <PR/libaudio.h>

void alSeqpStop(ALSeqPlayer* seqp) {
    ALEvent event;

    event.type = AL_SEQP_STOPPING_EVT;

    alEvtqPostEvent(&seqp->evtq, &event, 0);
}
