#include "common.h"

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80025E00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80025E6C.s")

typedef struct {
    u32 data_start_vrom;
    u32 data_end_vrom;
    u32 text_start_vrom;
    u32 text_end_vrom;
} struct_D_800CD08C;

extern struct_D_800CD08C D_800CD08C[];
extern u32 D_8013772C;
extern u32 D_80137730;

extern void DMA_ReadSync(u32 device_addr, void* vram_addr, u32 length);

#define VRAM_DATA (0x80192000)
#define VRAM_TEXT (0x80192100)

void func_80025EC4(u16 index) {
    s32 length0;
    s32 length1;
    
    index += 0;
    
    if (D_800CD08C[index].data_start_vrom != NULL) {
        length0 = D_800CD08C[index].data_end_vrom - D_800CD08C[index].data_start_vrom;
        D_8013772C = VRAM_DATA; // vram start
        D_80137730 = VRAM_DATA + length0; // vram end
        DMA_ReadSync(D_800CD08C[index].data_start_vrom, D_8013772C, length0);
        
        length1 = D_800CD08C[index].text_end_vrom - D_800CD08C[index].text_start_vrom;
        D_8013772C = VRAM_TEXT; // vram start
        D_80137730 = VRAM_TEXT + length1; // vram end
        DMA_ReadSync(D_800CD08C[index].text_start_vrom, D_8013772C, length1);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80025F70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_8002601C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_800260C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026174.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026220.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026428.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026494.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_8002653C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026584.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_800265FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026694.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_8002670C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026784.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_800267FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026874.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_8002694C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026A18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026B04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026BD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026C9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026D88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026E60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026F2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80027018.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_800270E4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_800271B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_8002729C.s")
