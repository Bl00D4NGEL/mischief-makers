#include <PR/libaudio.h>

void alSeqpSetVol(ALSeqPlayer* seqp, s16 vol) {
    ALEvent event;

    event.type = AL_SEQP_VOL_EVT;
    event.msg.spvol.vol = vol;

    alEvtqPostEvent(&seqp->evtq, &event, 0);
}
