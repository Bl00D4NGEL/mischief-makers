#ifndef GAMEDMA_H
#define GAMEDMA_H

#include "inttypes.h"

extern void DMA_ReadSync(u32 device_addr, void* vram_addr, u32 length);
extern void DMA_AwaitMessage(void);
extern void DMA_Read(u32 device_addr, void* vram_addr, u32 length);

#endif

