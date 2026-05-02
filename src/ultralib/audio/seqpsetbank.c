#include <PR/libaudio.h>

void alSeqpSetBank(ALSeqPlayer* seqp, ALBank* bank) {
    ALEvent event;

    event.type = AL_SEQP_BANK_EVT;
    event.msg.spbank.bank = bank;

    alEvtqPostEvent(&seqp->evtq, &event, 0);
}
