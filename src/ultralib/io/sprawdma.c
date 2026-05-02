#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"
#include "assert.h"

s32 __osSpRawStartDma(s32 direction, u32 devAddr, void* dramAddr, u32 size) {
    assert((devAddr & 0x7) == 0, "../io/sprawdma.c", 58);
    assert(((u32)dramAddr & 0x7) == 0, "../io/sprawdma.c", 59);
    assert((size & 0x7) == 0, "../io/sprawdma.c", 60);

    if (__osSpDeviceBusy()) {
        return -1;
    }

    IO_WRITE(SP_MEM_ADDR_REG, devAddr);
    IO_WRITE(SP_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));

    if (direction == OS_READ) {
        IO_WRITE(SP_WR_LEN_REG, size - 1);
    }
    else {
        IO_WRITE(SP_RD_LEN_REG, size - 1);
    }

    return 0;
}
