#include "PR/os_internal.h"
#include "PRinternal/siint.h"
#include "assert.h"

s32 __osSiRawWriteIo(u32 devAddr, u32 data) {
    assert((devAddr & 0x3) == 0, "../io/sirawwrite.c", 52);

    if (__osSiDeviceBusy()) {
        return -1;
    }

    IO_WRITE(devAddr, data);
    return 0;
}
