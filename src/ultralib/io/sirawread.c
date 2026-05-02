#include "PR/os_internal.h"
#include "PRinternal/siint.h"
#include "assert.h"

s32 __osSiRawReadIo(u32 devAddr, u32* data) {
    assert((devAddr & 0x3) == 0, "../io/sirawread.c", 51);
    assert(data != NULL, "../io/sirawread.c", 52);

    if (__osSiDeviceBusy()) {
        return -1;
    }

    *data = IO_READ(devAddr);
    return 0;
}
