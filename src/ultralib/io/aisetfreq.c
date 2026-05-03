#include <PR/rcp.h>
#include <PR/os_internal_error.h>
#include <PR/ultraerror.h>

extern s32 osViClock;

s32 osAiSetFrequency(u32 frequency) {
    register u32 dac_rate;
    register u8 bit_rate;
    register float temp;

    if (osViClock == VI_PAL_CLOCK) {
        if (frequency < AI_PAL_MIN_FREQ || frequency > AI_PAL_MAX_FREQ) {
            __osError(ERR_OSAISETFREQUENCY, 3, AI_PAL_MIN_FREQ, AI_PAL_MAX_FREQ, frequency);
            return -1;
        }
    }
    else if (osViClock == VI_MPAL_CLOCK) {
        if (frequency < AI_MPAL_MIN_FREQ || frequency > AI_MPAL_MAX_FREQ) {
            __osError(ERR_OSAISETFREQUENCY, 3, AI_MPAL_MIN_FREQ, AI_MPAL_MAX_FREQ, frequency);
            return -1;
        }
    }
    else {
        if (frequency < AI_NTSC_MIN_FREQ || frequency > AI_NTSC_MAX_FREQ) {
            __osError(ERR_OSAISETFREQUENCY, 3, AI_NTSC_MIN_FREQ, AI_NTSC_MAX_FREQ, frequency);
            return -1;
        }
    }

    temp = (osViClock / (float)frequency) + 0.5f;
    dac_rate = temp;

    if (dac_rate < AI_MIN_DAC_RATE) {
        return -1;
    }

    bit_rate = dac_rate / 66;
    if (bit_rate > AI_MAX_BIT_RATE) {
        bit_rate = AI_MAX_BIT_RATE;
    }

    IO_WRITE(AI_DACRATE_REG, dac_rate - 1);
    IO_WRITE(AI_BITRATE_REG, bit_rate - 1);
    IO_WRITE(AI_CONTROL_REG, AI_CONTROL_DMA_ON);

    return osViClock / (s32)dac_rate;
}
