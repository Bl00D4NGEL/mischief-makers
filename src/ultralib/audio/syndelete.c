#include "PR/libaudio.h"

void alSynDelete(ALSynth* drvr) {
    drvr->head = NULL;
}
