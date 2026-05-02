#include <PR/libaudio.h>
#include <audio/synthInternals.h>

Acmd* alAuxBusPull(void* filter, s16* outp, s32 outCount, s32 sampleOffset, Acmd* p) {
    Acmd* ptr = p;
    ALAuxBus* bus = (ALAuxBus*)filter;
    ALFilter** sources = bus->sources;
    s32 index;

    aClearBuffer(ptr++, AL_AUX_L_OUT, outCount << 1);
    aClearBuffer(ptr++, AL_AUX_R_OUT, outCount << 1);

    for (index = 0; index < bus->sourceCount; index++) {
        ptr = (sources[index]->handler)(sources[index], outp, outCount, sampleOffset, ptr);
    }

    return ptr;
}

s32 alAuxBusParam(void* filter, s32 paramID, void* param) {
    ALAuxBus* bus = (ALAuxBus*)filter;
    ALFilter** sources = bus->sources;

    switch (paramID) {
        case AL_FILTER_ADD_SOURCE:
            sources[bus->sourceCount++] = (ALFilter*)param;
            break;

        default:
            break;
    }

    return 0;
}
