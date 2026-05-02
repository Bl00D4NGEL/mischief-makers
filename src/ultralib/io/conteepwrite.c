#include <PR/os_internal.h>
#include "controller.h"
#include "siint.h"

void __osPackEepWriteData(u8 address, u8* buffer);

s32 osEepromWrite(OSMesgQueue* mq, u8 address, u8* buffer) {
    s32 ret = 0;
    int index;
    u8* ptr;
    __OSContEepromFormat eeprom_format;
    OSContStatus cont_status;

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

    __osPackEepWriteData(address, buffer);
    ret = __osSiRawStartDma(OS_WRITE, &__osEepPifRam);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);

    for (index = 0; index < ARRLEN(__osEepPifRam.raw); index++) {
        __osEepPifRam.raw[index] = CONT_CMD_NOP;
    }
    __osEepPifRam.pifstatus = 0;

    ret = __osSiRawStartDma(OS_READ, &__osEepPifRam);
    __osContLastCmd = CONT_CMD_WRITE_EEPROM;
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);

    for (index = 0; index < MAXCONTROLLERS; index++) {
        ptr++;
    }

    eeprom_format = *(__OSContEepromFormat*)ptr;
    ret = CHNL_ERR(eeprom_format);

    __osSiRelAccess();
    return ret;
}

void __osPackEepWriteData(u8 address, u8* buffer) {
    u8* ptr = (u8*)&__osEepPifRam.ramarray;
    __OSContEepromFormat eeprom_format;
    int index;

    for (index = 0; index < ARRLEN(__osEepPifRam.raw); index++) {
        __osEepPifRam.raw[index] = CONT_CMD_NOP;
    }
    __osEepPifRam.pifstatus = CONT_CMD_EXE;

    eeprom_format.txsize = CONT_CMD_WRITE_EEPROM_TX;
    eeprom_format.rxsize = CONT_CMD_WRITE_EEPROM_RX;
    eeprom_format.cmd = CONT_CMD_WRITE_EEPROM;
    eeprom_format.address = address;

    for (index = 0; index < ARRLEN(eeprom_format.data); index++) {
        eeprom_format.data[index] = *buffer++;
    }

    for (index = 0; index < MAXCONTROLLERS; index++) {
        *ptr++ = 0;
    }

    *(__OSContEepromFormat*)ptr = eeprom_format;
    ptr += sizeof(__OSContEepromFormat);
    *ptr = CONT_CMD_END;
}

s32 __osEepStatus(OSMesgQueue* mq, OSContStatus* data) {
    s32 ret = 0;
    int index;
    u8* ptr = (u8*)__osEepPifRam.ramarray;
    __OSContRequesFormat request_format;

    for (index = 0; index < ARRLEN(__osEepPifRam.raw); index++) {
        __osEepPifRam.raw[index] = 0;
    }

    __osEepPifRam.pifstatus = CONT_CMD_EXE;
    ptr = (u8*)__osEepPifRam.ramarray;

    for (index = 0; index < MAXCONTROLLERS; index++) {
        *ptr++ = CONT_CMD_REQUEST_STATUS;
    }

    request_format.dummy = CONT_CMD_NOP;
    request_format.txsize = CONT_CMD_REQUEST_STATUS_TX;
    request_format.rxsize = CONT_CMD_REQUEST_STATUS_RX;
    request_format.cmd = CONT_CMD_REQUEST_STATUS;
    request_format.typeh = CONT_CMD_NOP;
    request_format.typel = CONT_CMD_NOP;
    request_format.status = CONT_CMD_NOP;
    request_format.dummy1 = CONT_CMD_NOP;

    *(__OSContRequesFormat*)ptr = request_format;
    ptr += sizeof(__OSContRequesFormat);
    *ptr = CONT_CMD_END;

    ret = __osSiRawStartDma(OS_WRITE, &__osEepPifRam);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    __osContLastCmd = CONT_CMD_WRITE_EEPROM;

    ret = __osSiRawStartDma(OS_READ, &__osEepPifRam);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);

    if (ret != 0) {
        return ret;
    }

    ptr = (u8*)&__osEepPifRam;

    for (index = 0; index < MAXCONTROLLERS; index++) {
        *ptr++ = 0;
    }

    request_format = *(__OSContRequesFormat*)ptr;
    data->errno = CHNL_ERR(request_format);
    data->type = (request_format.typel << 8) | request_format.typeh;
    data->status = request_format.status;

    if (data->errno != 0) {
        return data->errno;
    }

    return 0;
}
