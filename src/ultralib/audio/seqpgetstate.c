#include <PR/libaudio.h>

s32 alSeqpGetState(ALSeqPlayer* seqp) {
    return seqp->state;
}
