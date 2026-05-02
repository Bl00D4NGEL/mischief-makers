#include <PR/os_internal.h>
#include "controller.h"
#include "siint.h"

static void __osPackReadData(void);

s32 osContStartReadData(OSMesgQueue* mq) {
    s32 ret = 0;
    s32 index;

    __osSiGetAccess();

    if (__osContLastCmd != CONT_CMD_READ_BUTTON) {
        __osPackReadData();
        ret = __osSiRawStartDma(OS_WRITE, __osContPifRam.ramarray);
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    }

    for (index = 0; index < ARRLEN(__osContPifRam.raw); index++) {
        __osContPifRam.ramarray[index] = CONT_CMD_NOP;
    }
    __osContPifRam.pifstatus = 0;

    ret = __osSiRawStartDma(OS_READ, __osContPifRam.ramarray);
    __osContLastCmd = CONT_CMD_READ_BUTTON;
    __osSiRelAccess();

    return ret;
}

void osContGetReadData(OSContPad* data) {
    u8* ptr = (u8*)__osContPifRam.ramarray;
    __OSContReadFormat read_format;
    s32 index;

    for (index = 0; index < __osMaxControllers; index++, ptr += sizeof(__OSContReadFormat), data++) {
        read_format = *(__OSContReadFormat*)ptr;
        data->errno = CHNL_ERR(read_format);

        if (data->errno != 0) {
            continue;
        }

        data->button = read_format.button;
        data->stick_x = read_format.stick_x;
        data->stick_y = read_format.stick_y;
    }
}

static void __osPackReadData(void) {
    u8* ptr = (u8*)__osContPifRam.ramarray;
    __OSContReadFormat read_format;
    s32 index;

    for (index = 0; index < ARRLEN(__osContPifRam.raw); index++) {
        __osContPifRam.ramarray[index] = 0;
    }

    __osContPifRam.pifstatus = CONT_CMD_EXE;
    read_format.dummy = CONT_CMD_NOP;
    read_format.txsize = CONT_CMD_READ_BUTTON_TX;
    read_format.rxsize = CONT_CMD_READ_BUTTON_RX;
    read_format.cmd = CONT_CMD_READ_BUTTON;
    read_format.button = 0xFFFF;
    read_format.stick_x = -1;
    read_format.stick_y = -1;

    for (index = 0; index < __osMaxControllers; index++) {
        *(__OSContReadFormat*)ptr = read_format;
        ptr += sizeof(__OSContReadFormat);
    }

    *ptr = CONT_CMD_END;
}
