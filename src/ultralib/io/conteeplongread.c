#include <PR/os_internal.h>
#include <PRinternal/controller.h>

#define OS_USEC_TO_CYCLES_RUNTIME(usec) (((u64)(usec) * osClockRate) / 1000000)

s32 osEepromLongRead(OSMesgQueue* mq, u8 address, u8* buffer, int length) {
    s32 ret = 0;

    if (address > EEPROM_MAXBLOCKS) {
        return -1;
    }

    while (length > 0) {
        ERRCK(osEepromRead(mq, address, buffer));
        length -= EEPROM_BLOCK_SIZE;
        address++;
        buffer += EEPROM_BLOCK_SIZE;

        osSetTimer(&__osEepromTimer, OS_USEC_TO_CYCLES_RUNTIME(12000), 0, &__osEepromTimerQ, &__osEepromTimerMsg);
        osRecvMesg(&__osEepromTimerQ, NULL, OS_MESG_BLOCK);
    }

    return ret;
}
