#include <PR/libaudio.h>

void alSeqpSetSeq(ALSeqPlayer* seqp, ALSeq* seq) {
    ALEvent event;

    event.type = AL_SEQP_SEQ_EVT;
    event.msg.spseq.seq = seq;

    alEvtqPostEvent(&seqp->evtq, &event, 0);
}
