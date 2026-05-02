#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"
#include "assert.h"

s32 __osSpRawReadIo(u32 devAddr, u32* data) {
    assert((devAddr & 0x3) == 0, "../io/sprawread.c", 51);
    assert(data != NULL, "../io/sprawread.c", 52);

    if (__osSpDeviceBusy()) {
        return -1;
    }

    *data = IO_READ(devAddr);
    return 0;
}
