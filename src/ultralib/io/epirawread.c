#include "PR/ultraerror.h"
#include "PRinternal/piint.h"

extern void __assert(const char* expression, const char* file, int line);

s32 __osEPiRawReadIo(OSPiHandle* pi_handle, u32 device_address, u32* data) {
    register u32 status;

    if (device_address & 0x3) {
        __osError(ERR_OSPIRAWREADIO, 1, device_address);
        return -1;
    }

    if (data != NULL) {
    }
    else {
        __assert("data != NULL", "../io/epirawread.c", 41);
    }

    WAIT_ON_IOBUSY(status);
    *data = IO_READ(pi_handle->baseAddress | device_address);

    return 0;
}
