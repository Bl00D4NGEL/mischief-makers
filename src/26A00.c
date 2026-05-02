#include "common.h"

typedef struct {
    u32 dispatch_table_rom_start;
    u32 dispatch_table_rom_end;
    u32 object_rom_start;
    u32 object_rom_end;
} OverlayLoadEntry;

extern OverlayLoadEntry D_800CD08C[];
extern OverlayLoadEntry D_800CD60C[];
extern OverlayLoadEntry D_800CDB8C[];
extern OverlayLoadEntry D_800CE10C[];
extern OverlayLoadEntry D_800CE68C[];
extern u32 D_8013772C;
extern u32 D_80137730;
extern u32 D_80137734;
extern u32 D_80137738;
extern u32 D_8013773C;
extern u32 D_80137740;
extern u32 D_80137744;
extern u32 D_80137748;
extern u32 D_8013774C;
extern u32 D_80137750;

extern void DMA_ReadSync(u32 device_addr, void* vram_addr, u32 length);

#define OVERLAY0_DISPATCH_TABLE_VRAM (0x80192000)
#define OVERLAY0_OBJECT_VRAM         (0x80192100)
#define OVERLAY1_DISPATCH_TABLE_VRAM (0x8019B000)
#define OVERLAY1_OBJECT_VRAM         (0x8019B100)
#define OVERLAY2_DISPATCH_TABLE_VRAM (0x801A6800)
#define OVERLAY2_OBJECT_VRAM         (0x801A6900)
#define OVERLAY3_DISPATCH_TABLE_VRAM (0x801B0800)
#define OVERLAY3_OBJECT_VRAM         (0x801B0900)
#define OVERLAY4_DISPATCH_TABLE_VRAM (0x801B9800)
#define OVERLAY4_OBJECT_VRAM         (0x801B9900)


#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80025E00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80025E6C.s")

void func_80025EC4(u16 index) {
    s32 length0;
    s32 length1;

    index += 0;

    if (D_800CD08C[index].dispatch_table_rom_start != NULL) {
        length0 = D_800CD08C[index].dispatch_table_rom_end - D_800CD08C[index].dispatch_table_rom_start;
        D_8013772C = OVERLAY0_DISPATCH_TABLE_VRAM;
        D_80137730 = OVERLAY0_DISPATCH_TABLE_VRAM + length0;
        DMA_ReadSync(D_800CD08C[index].dispatch_table_rom_start, (void*)D_8013772C, length0);

        length1 = D_800CD08C[index].object_rom_end - D_800CD08C[index].object_rom_start;
        D_8013772C = OVERLAY0_OBJECT_VRAM;
        D_80137730 = OVERLAY0_OBJECT_VRAM + length1;
        DMA_ReadSync(D_800CD08C[index].object_rom_start, (void*)D_8013772C, length1);
    }
}

void func_80025F70(u16 index) {
    s32 length0;
    s32 length1;

    index += 0;

    if (D_800CD60C[index].dispatch_table_rom_start != NULL) {
        length0 = D_800CD60C[index].dispatch_table_rom_end - D_800CD60C[index].dispatch_table_rom_start;
        D_80137734 = OVERLAY1_DISPATCH_TABLE_VRAM;
        D_80137738 = OVERLAY1_DISPATCH_TABLE_VRAM + length0;
        DMA_ReadSync(D_800CD60C[index].dispatch_table_rom_start, (void*)D_80137734, length0);

        length1 = D_800CD60C[index].object_rom_end - D_800CD60C[index].object_rom_start;
        D_80137734 = OVERLAY1_OBJECT_VRAM;
        D_80137738 = OVERLAY1_OBJECT_VRAM + length1;
        DMA_ReadSync(D_800CD60C[index].object_rom_start, (void*)D_80137734, length1);
    }
}

void func_8002601C(u16 index) {
    s32 length0;
    s32 length1;

    index += 0;

    if (D_800CDB8C[index].dispatch_table_rom_start != NULL) {
        length0 = D_800CDB8C[index].dispatch_table_rom_end - D_800CDB8C[index].dispatch_table_rom_start;
        D_8013773C = OVERLAY2_DISPATCH_TABLE_VRAM;
        D_80137740 = OVERLAY2_DISPATCH_TABLE_VRAM + length0;
        DMA_ReadSync(D_800CDB8C[index].dispatch_table_rom_start, (void*)D_8013773C, length0);

        length1 = D_800CDB8C[index].object_rom_end - D_800CDB8C[index].object_rom_start;
        D_8013773C = OVERLAY2_OBJECT_VRAM;
        D_80137740 = OVERLAY2_OBJECT_VRAM + length1;
        DMA_ReadSync(D_800CDB8C[index].object_rom_start, (void*)D_8013773C, length1);
    }
}

void func_800260C8(u16 index) {
    s32 length0;
    s32 length1;

    index += 0;

    if (D_800CE10C[index].dispatch_table_rom_start != NULL) {
        length0 = D_800CE10C[index].dispatch_table_rom_end - D_800CE10C[index].dispatch_table_rom_start;
        D_80137744 = OVERLAY3_DISPATCH_TABLE_VRAM;
        D_80137748 = OVERLAY3_DISPATCH_TABLE_VRAM + length0;
        DMA_ReadSync(D_800CE10C[index].dispatch_table_rom_start, (void*)D_80137744, length0);

        length1 = D_800CE10C[index].object_rom_end - D_800CE10C[index].object_rom_start;
        D_80137744 = OVERLAY3_OBJECT_VRAM;
        D_80137748 = OVERLAY3_OBJECT_VRAM + length1;
        DMA_ReadSync(D_800CE10C[index].object_rom_start, (void*)D_80137744, length1);
    }
}

void func_80026174(u16 index) {
    s32 length0;
    s32 length1;

    index += 0;

    if (D_800CE68C[index].dispatch_table_rom_start != NULL) {
        length0 = D_800CE68C[index].dispatch_table_rom_end - D_800CE68C[index].dispatch_table_rom_start;
        D_8013774C = OVERLAY4_DISPATCH_TABLE_VRAM;
        D_80137750 = OVERLAY4_DISPATCH_TABLE_VRAM + length0;
        DMA_ReadSync(D_800CE68C[index].dispatch_table_rom_start, (void*)D_8013774C, length0);

        length1 = D_800CE68C[index].object_rom_end - D_800CE68C[index].object_rom_start;
        D_8013774C = OVERLAY4_OBJECT_VRAM;
        D_80137750 = OVERLAY4_OBJECT_VRAM + length1;
        DMA_ReadSync(D_800CE68C[index].object_rom_start, (void*)D_8013774C, length1);
    }
}

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
