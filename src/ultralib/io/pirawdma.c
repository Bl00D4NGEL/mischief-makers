#include "piint.h"
#include "PR/ultraerror.h"

s32 osPiRawStartDma(s32 direction, u32 device_address, void* dram_address, u32 size) {
    register u32 status;

    if ((direction != OS_READ) && (direction != OS_WRITE)) {
        __osError(ERR_OSPIRAWSTARTDMA_DIR, 1, direction);
        return -1;
    }

    if (device_address & 1) {
        __osError(ERR_OSPIRAWSTARTDMA_DEVADDR, 1, device_address);
        return -1;
    }

    if ((u32)dram_address & 7) {
        __osError(ERR_OSPIRAWSTARTDMA_ADDR, 1, dram_address);
        return -1;
    }

    if (size & 1) {
        __osError(ERR_OSPIRAWSTARTDMA_SIZE, 1, size);
        return -1;
    }

    if ((size == 0) || (size > (16 * 1024 * 1024))) {
        __osError(ERR_OSPIRAWSTARTDMA_RANGE, 1, size);
        return -1;
    }

    WAIT_ON_IOBUSY(status);

    IO_WRITE(PI_DRAM_ADDR_REG, osVirtualToPhysical(dram_address));
    IO_WRITE(PI_CART_ADDR_REG, K1_TO_PHYS((u32)osRomBase | device_address));

    switch (direction) {
        case OS_READ:
            IO_WRITE(PI_WR_LEN_REG, size - 1);
            break;

        case OS_WRITE:
            IO_WRITE(PI_RD_LEN_REG, size - 1);
            break;

        default:
            return -1;
    }

    return 0;
}
