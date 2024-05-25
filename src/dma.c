#include <ultra64.h>
#include "inttypes.h"

void DMA_ReadSync(u32 device_addr, void* vram_addr, u32 length);
void DMA_AwaitMessage(void);
void DMA_Read(u32 device_addr, void* vram_addr, u32 length);

extern OSMesgQueue gDmaMesgQ;

// Synchronously (blocking) read length bytes from rom_addr to vram_addr
void DMA_ReadSync(u32 rom_addr, void* vram_addr, u32 length) {
    OSIoMesg iomesg;
    OSMesg dummy;

    osInvalDCache(vram_addr, length);
    osPiStartDma(&iomesg, OS_MESG_PRI_NORMAL, OS_READ, rom_addr, vram_addr, length, &gDmaMesgQ);
    osRecvMesg(&gDmaMesgQ, &dummy, OS_MESG_BLOCK);
}

// Synchronously (blocking) wait to receive a message event on gDmaMesgQ
void DMA_AwaitMessage(void) {
    OSMesg dummy;
    osRecvMesg(&gDmaMesgQ, &dummy, OS_MESG_BLOCK);
}

// Start reading length bytes from rom_addr to vram_addr
void DMA_Read(u32 device_addr, void* vram_addr, u32 length) {
    OSIoMesg iomesg;

    osInvalDCache(vram_addr, length);
    osPiStartDma(&iomesg, OS_MESG_PRI_NORMAL, OS_READ, device_addr, vram_addr, length, &gDmaMesgQ);
}

