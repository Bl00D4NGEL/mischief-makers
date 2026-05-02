#include <PRinternal/controller.h>
#include <PRinternal/siint.h>

s32 osEepromProbe(OSMesgQueue* mq) {
    s32 status = 0;
    OSContStatus cont_status;

    __osSiGetAccess();
    status = __osEepStatus(mq, &cont_status);

    if ((status == 0) && ((cont_status.type & CONT_EEPROM) != 0)) {
        status = 1;
    }
    else {
        status = 0;
    }

    __osSiRelAccess();
    return status;
}
