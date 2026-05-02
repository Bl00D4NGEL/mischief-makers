#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/R4300.h"
#include "PR/rcp.h"
#include "piint.h"

OSPiHandle CartRomHandle ALIGNED(8);

OSPiHandle* osCartRomInit(void) {
    u32 domain;
    u32 saved_mask;

    domain = 0;

    if (CartRomHandle.baseAddress == PHYS_TO_K1(PI_DOM1_ADDR2)) {
        return &CartRomHandle;
    }

    CartRomHandle.type = DEVICE_TYPE_CART;
    CartRomHandle.baseAddress = PHYS_TO_K1(PI_DOM1_ADDR2);
    osPiRawReadIo(0, &domain);
    CartRomHandle.latency = domain & 0xFF;
    CartRomHandle.pulse = (domain >> 8) & 0xFF;
    CartRomHandle.pageSize = (domain >> 16) & 0xF;
    CartRomHandle.relDuration = (domain >> 20) & 0xF;
    CartRomHandle.domain = PI_DOMAIN1;

    bzero(&CartRomHandle.transferInfo, sizeof(__OSTranxInfo));

    saved_mask = __osDisableInt();
    CartRomHandle.next = __osPiTable;
    __osPiTable = &CartRomHandle;
    __osRestoreInt(saved_mask);

    return &CartRomHandle;
}
