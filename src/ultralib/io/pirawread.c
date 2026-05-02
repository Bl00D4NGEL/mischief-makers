#include "PRinternal/piint.h"
#include "PR/ultraerror.h"
#include "assert.h"

s32 osPiRawReadIo(u32 devAddr, u32* data) {
    register u32 stat;

    if (devAddr & 0x3) {
        __osError(ERR_OSPIRAWREADIO, 1, devAddr);
        return -1;
    }

    assert(data != NULL, "../io/pirawread.c", 61);

    WAIT_ON_IOBUSY(stat);
    *data = IO_READ((u32)osRomBase | devAddr);

    return 0;
}
