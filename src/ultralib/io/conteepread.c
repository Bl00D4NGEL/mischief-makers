#include <PR/os_internal.h>
#include "controller.h"
#include "siint.h"

OSPifRam __osEepPifRam;

void __osPackEepReadData(u8 address);

s32 osEepromRead(OSMesgQueue* mq, u8 address, u8* buffer) {
    s32 ret = 0;
    int index = 0;
    u8* ptr;
    OSContStatus cont_status;
    __OSContEepromFormat eeprom_format;

    ptr = (u8*)&__osEepPifRam.ramarray;

    if (address > EEPROM_MAXBLOCKS) {
        return -1;
    }

    __osSiGetAccess();
    ret = __osEepStatus(mq, &cont_status);

    if ((ret != 0) || (cont_status.type != CONT_EEPROM)) {
        return CONT_NO_RESPONSE_ERROR;
    }

    while (cont_status.status & CONT_EEPROM_BUSY) {
        __osEepStatus(mq, &cont_status);
    }

    __osPackEepReadData(address);
    ret = __osSiRawStartDma(OS_WRITE, &__osEepPifRam);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);

    for (index = 0; index < ARRLEN(__osEepPifRam.raw); index++) {
        __osEepPifRam.raw[index] = CONT_CMD_NOP;
    }
    __osEepPifRam.pifstatus = 0;

    ret = __osSiRawStartDma(OS_READ, &__osEepPifRam);
    __osContLastCmd = CONT_CMD_READ_EEPROM;
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);

    for (index = 0; index < MAXCONTROLLERS; index++) {
        ptr++;
    }

    eeprom_format = *(__OSContEepromFormat*)ptr;
    ret = CHNL_ERR(eeprom_format);

    if (ret == 0) {
        for (index = 0; index < ARRLEN(eeprom_format.data); index++) {
            *buffer++ = eeprom_format.data[index];
        }
    }

    __osSiRelAccess();
    return ret;
}

void __osPackEepReadData(u8 address) {
    u8* ptr = (u8*)&__osEepPifRam.ramarray;
    __OSContEepromFormat eeprom_format;
    int index;

    for (index = 0; index < ARRLEN(__osEepPifRam.raw); index++) {
        __osEepPifRam.raw[index] = CONT_CMD_NOP;
    }
    __osEepPifRam.pifstatus = CONT_CMD_EXE;

    eeprom_format.txsize = CONT_CMD_READ_EEPROM_TX;
    eeprom_format.rxsize = CONT_CMD_READ_EEPROM_RX;
    eeprom_format.cmd = CONT_CMD_READ_EEPROM;
    eeprom_format.address = address;

    for (index = 0; index < ARRLEN(eeprom_format.data); index++) {
        eeprom_format.data[index] = 0;
    }

    for (index = 0; index < MAXCONTROLLERS; index++) {
        *ptr++ = 0;
    }

    *(__OSContEepromFormat*)ptr = eeprom_format;
    ptr += sizeof(__OSContEepromFormat);
    *ptr = CONT_CMD_END;
}
