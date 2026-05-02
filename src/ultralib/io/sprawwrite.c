#include <assert.h>
#include <PR/os_internal.h>
#include <PR/rcp.h>
#include <PRinternal/osint.h>

s32 __osSpRawWriteIo(u32 devAddr, u32 data) {
    assert((devAddr & 0x3) == 0, "../io/sprawwrite.c", 51);

    if (__osSpDeviceBusy()) {
        return -1;
    }

    IO_WRITE(devAddr, data);
    return 0;
}
