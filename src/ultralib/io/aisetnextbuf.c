#include <PR/os_internal.h>
#include <PR/rcp.h>
#include <PR/ultraerror.h>
#include <PRinternal/osint.h>

static u8 hdwrBugFlag = FALSE;

s32 osAiSetNextBuffer(void* buf_ptr, u32 size) {
    char* adjusted_buf_ptr;

    if ((u32)buf_ptr & (8 - 1)) {
        __osError(ERR_OSAISETNEXTBUFFER_ADDR, 1, buf_ptr);
        return -1;
    }

    if (size & (8 - 1)) {
        __osError(ERR_OSAISETNEXTBUFFER_SIZE, 1, size);
        return -1;
    }

    adjusted_buf_ptr = buf_ptr;
    if (hdwrBugFlag) {
        adjusted_buf_ptr = (u8*)buf_ptr - 0x2000;
    }

    if ((((u32)buf_ptr + size) & 0x3FFF) == 0x2000) {
        hdwrBugFlag = TRUE;
    }
    else {
        hdwrBugFlag = FALSE;
    }

    if (__osAiDeviceBusy()) {
        return -1;
    }

    IO_WRITE(AI_DRAM_ADDR_REG, osVirtualToPhysical(adjusted_buf_ptr));
    IO_WRITE(AI_LEN_REG, size);
    return 0;
}
