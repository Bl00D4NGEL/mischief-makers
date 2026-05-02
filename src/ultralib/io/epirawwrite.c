#include "PR/ultraerror.h"
#include "PRinternal/piint.h"

s32 __osEPiRawWriteIo(OSPiHandle* pi_handle, u32 device_address, u32 data) {
    register u32 status;

    if (device_address & 0x3) {
        __osError(ERR_OSPIRAWWRITEIO, 1, device_address);
        return -1;
    }

    WAIT_ON_IOBUSY(status);
    IO_WRITE(pi_handle->baseAddress | device_address, data);

    return 0;
}
