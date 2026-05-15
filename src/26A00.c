#include "common.h"
#include "dma.h"

typedef struct {
    u8* dispatch_table_rom_start;
    u8* dispatch_table_rom_end;
    u8* object_rom_start;
    u8* object_rom_end;
} OverlayLoadEntry;

extern OverlayLoadEntry D_800CD08C[];
extern OverlayLoadEntry D_800CD60C[];
extern OverlayLoadEntry D_800CDB8C[];
extern OverlayLoadEntry D_800CE10C[];
extern OverlayLoadEntry D_800CE68C[];
extern u32 D_800CEC0C[];
extern u32 D_800CEC10[];
extern u32 D_800CEED0[];
extern u32 D_800CF2F4[];
extern u32 D_800CF5B8[];
extern u32 D_800CF87C[];
extern u32 D_800CFB40[];
extern u32 D_800CFE04[];
extern u16 D_800CBF50;
extern u16 D_800CBF54;
extern u16 D_800D28E4;
extern u32 D_800D0E84[];
extern u32 D_800D0E88[];
extern u32 D_800D0E8C[];
extern u32 D_800D0E90[];
extern u32 D_800D0E94[];
extern u32 D_800D0E98[];
extern u8 D_800DE348[];
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
extern u32 D_801376C0;
extern u32 D_801376C4;
extern u32 D_801376C8;
extern u32 D_801376CC;
extern u32 D_801376D0;
extern u32 D_801376D4;
extern u32 D_801376D8;
extern u32 D_801376DC;
extern u32 D_801376E0;
extern s32 D_801376E4;
extern u32 D_801376E8;
extern u32 D_801376EC;
extern u32 D_801376F0;
extern u32 D_801376F4;
extern u32 D_801376F8;
extern u32 D_801376FC;
extern u32 D_80137700;
extern u32 D_80137704;
extern u32 D_80137708;
extern u32 D_8013770C;
extern u32 D_80137710;
extern u32 D_80137714;
extern u32 D_80137718;
extern u32 D_8013771C;
extern u32 D_80137720;
extern u32 D_80137724;
extern u32 D_80137728;
extern u32 D_8013776C;
extern u32 D_80137770;
extern u32 D_80137774;
extern u32 D_80137778;
extern u32 D_8013777C;
extern u32 D_80137780;
extern u32 D_80137784;
extern u32 D_80137788;
extern u32 D_8013778C;
extern u32 D_80137790;
extern u8 D_1000000[];
extern u8 D_10CE330[];
extern u8 D_119EB70;
extern u8 D_11BD940;
extern u8 D_11C1D30;
extern u8 D_4000000;
extern u8 D_407C860;
extern u8 D_4081620;
extern u8 D_4091D20;
extern u8 D_40E17F0;
extern u8 D_40F50F0[];
extern u8 D_40F52F8;

extern s32 Trouble_RLE_Type1(u8* src, u8* dst);

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

#define RLE_SCRATCH_HIGH      ((u8*)0x80259000)
#define RLE_SCRATCH_LOW       ((u8*)0x801C4400)
#define MAP_BANK_DEST         ((u8*)0x80380000)
#define MAP_BANK_DEST_ALT     ((u8*)0x8038DE00)
#define MAP_BANK_DATA_DEST    ((u8*)0x80388000)
#define ACTOR_LIST_DEST       ((u8*)0x801C3000)
#define COLLISION_DEST        ((u8*)0x801C4300)
#define COLLISION_SCRATCH_ALT ((u8*)0x801D4400)
#define STAGE_TABLE0_DEST     ((u8*)0x801C2000)
#define STAGE_TABLE1_DEST     ((u8*)0x801C4200)
#define STAGE_TABLE2_DEST     ((u8*)0x801C1000)
#define ASSET_DEST0           ((u8*)0x802CA500)
#define ASSET_DEST1           ((u8*)0x802E0000)
#define ASSET_DEST2           ((u8*)0x80300000)
#define ASSET_DEST3           ((u8*)0x80320000)
#define ASSET_DEST4           ((u8*)0x80340000)
#define ASSET_DEST5           ((u8*)0x80360000)
#define ASSET_DEST6           ((u8*)0x803CE800)

void func_80025E00(void) {
    D_801376DC = 0x80200400;
    DMA_ReadSync(gfx_common_gfx_resources_1E6900_80200400_ROM_START, (u8*)0x80267DC0,
                 gfx_common_gfx_resources_1E6900_80200400_ROM_END - gfx_common_gfx_resources_1E6900_80200400_ROM_START);
    D_801376E0 = 0x80200400 + Trouble_RLE_Type1((u8*)0x80267DC0, (u8*)0x80200400);
}

void func_80025E6C(void) {
    u32 size = gfx_clancer_ROM_END - gfx_clancer_ROM_START;
    D_801376E4 = 0x80296000;
    D_801376E8 = size + 0x80296000;
    DMA_ReadSync(gfx_clancer_ROM_START, (u8*)D_801376E4, gfx_clancer_ROM_END - gfx_clancer_ROM_START);
}

void func_80025EC4(u16 index) {
    s32 length0;
    s32 length1;

    index += 0;

    if (D_800CD08C[index].dispatch_table_rom_start != NULL) {
        length0 = D_800CD08C[index].dispatch_table_rom_end - D_800CD08C[index].dispatch_table_rom_start;
        D_8013772C = OVERLAY0_DISPATCH_TABLE_VRAM;
        D_80137730 = OVERLAY0_DISPATCH_TABLE_VRAM + length0;
        DMA_ReadSync(D_800CD08C[index].dispatch_table_rom_start, (u8*)D_8013772C, length0);

        length1 = D_800CD08C[index].object_rom_end - D_800CD08C[index].object_rom_start;
        D_8013772C = OVERLAY0_OBJECT_VRAM;
        D_80137730 = OVERLAY0_OBJECT_VRAM + length1;
        DMA_ReadSync(D_800CD08C[index].object_rom_start, (u8*)D_8013772C, length1);
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
        DMA_ReadSync(D_800CD60C[index].dispatch_table_rom_start, (u8*)D_80137734, length0);

        length1 = D_800CD60C[index].object_rom_end - D_800CD60C[index].object_rom_start;
        D_80137734 = OVERLAY1_OBJECT_VRAM;
        D_80137738 = OVERLAY1_OBJECT_VRAM + length1;
        DMA_ReadSync(D_800CD60C[index].object_rom_start, (u8*)D_80137734, length1);
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
        DMA_ReadSync(D_800CDB8C[index].dispatch_table_rom_start, (u8*)D_8013773C, length0);

        length1 = D_800CDB8C[index].object_rom_end - D_800CDB8C[index].object_rom_start;
        D_8013773C = OVERLAY2_OBJECT_VRAM;
        D_80137740 = OVERLAY2_OBJECT_VRAM + length1;
        DMA_ReadSync(D_800CDB8C[index].object_rom_start, (u8*)D_8013773C, length1);
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
        DMA_ReadSync(D_800CE10C[index].dispatch_table_rom_start, (u8*)D_80137744, length0);

        length1 = D_800CE10C[index].object_rom_end - D_800CE10C[index].object_rom_start;
        D_80137744 = OVERLAY3_OBJECT_VRAM;
        D_80137748 = OVERLAY3_OBJECT_VRAM + length1;
        DMA_ReadSync(D_800CE10C[index].object_rom_start, (u8*)D_80137744, length1);
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
        DMA_ReadSync(D_800CE68C[index].dispatch_table_rom_start, (u8*)D_8013774C, length0);

        length1 = D_800CE68C[index].object_rom_end - D_800CE68C[index].object_rom_start;
        D_8013774C = OVERLAY4_OBJECT_VRAM;
        D_80137750 = OVERLAY4_OBJECT_VRAM + length1;
        DMA_ReadSync(D_800CE68C[index].object_rom_start, (u8*)D_8013774C, length1);
    }
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/3R7AG
void func_80026220(u16 index) {
    if ((index == 2) || (D_800D28E4 == 0x66) || (gGameState == 8) || (gGameState == 9)) {
        DMA_ReadSync(((D_800CEC0C[5] - D_800CEC0C[0]) + (u32)bin_F0100_ROM_START), RLE_SCRATCH_LOW, D_800CEC0C[6] - D_800CEC0C[5]);
        D_80137724 = (u32)MAP_BANK_DEST;
        D_80137728 = (u32)MAP_BANK_DEST + Trouble_RLE_Type1(RLE_SCRATCH_LOW, MAP_BANK_DEST);
        DMA_ReadSync(bin_F00D0_ROM_START, MAP_BANK_DATA_DEST, bin_F00D0_ROM_END - bin_F00D0_ROM_START);
        return;
    }

    if (D_800CEC0C[index * 2 + 1] == (u32)&D_407C860) {
        DMA_ReadSync(((D_800CEC0C[index * 2 + 1] - D_800CEC0C[0]) + (u32)bin_F0100_ROM_START), RLE_SCRATCH_HIGH, (u32)D_4081620 - D_800CEC0C[index * 2 + 1]);
        D_80137724 = (u32)MAP_BANK_DEST;
        D_80137728 = (u32)MAP_BANK_DEST + Trouble_RLE_Type1(RLE_SCRATCH_HIGH, MAP_BANK_DEST);

        DMA_ReadSync(((D_4081620 - D_800CEC0C[0]) + (u32)bin_F0100_ROM_START), RLE_SCRATCH_HIGH, D_4091D20 - (u32)D_4081620);
        D_80137724 = (u32)MAP_BANK_DEST_ALT;
        D_80137728 = (u32)MAP_BANK_DEST_ALT + Trouble_RLE_Type1(RLE_SCRATCH_HIGH, MAP_BANK_DEST_ALT);
    }
    else {
        DMA_ReadSync(((D_4081620 - D_800CEC0C[0]) + (u32)bin_F0100_ROM_START), RLE_SCRATCH_HIGH, D_800CEC0C[index * 2 + 2] - (u32)D_4081620);
        D_80137724 = (u32)MAP_BANK_DEST;
        D_80137728 = (u32)MAP_BANK_DEST + Trouble_RLE_Type1(RLE_SCRATCH_HIGH, MAP_BANK_DEST);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026220.s")
#endif

void func_80026428(u16 index) {
    u32 start;
    u32* entry;

    entry = &D_800CEC0C[index * 2];
    start = entry[1];
    DMA_ReadSync((void*)((start - D_800CEC0C[0]) + (u32)bin_F0100_ROM_START), RLE_SCRATCH_LOW,
                 entry[2] - start);
    Trouble_RLE_Type1(RLE_SCRATCH_LOW, MAP_BANK_DEST);
}

void func_80026494(u16 index, u16 offset) {
    u32* entry = &D_800CEC0C[index * 2];
    DMA_ReadSync(((entry[1] - D_800CEC0C[0]) + bin_F0100_ROM_START), RLE_SCRATCH_LOW, entry[2] - entry[1]);
    Trouble_RLE_Type1(RLE_SCRATCH_LOW, 0x803A0600);
    DMA_ReadSync(((D_40F50F0 - D_800CEC0C[0]) + (u32)bin_F0100_ROM_START), 0x80380000 + offset, 0x200);
}

void func_8002653C(void) {
    u32 source;

    source = (u32)bin_F0100_ROM_START;
    source += (u32)&D_40F52F8;
    DMA_ReadSync(source - (u32)&D_4000000, D_800DE348, 0x1500);
}

void func_80026584(u16 index) {
    u32 start;
    u32 length;
    u32 source;
    s32 new_var;
    u32* dest_ptr;
    u32* entry;

    entry = &D_800CFB40[index * 2];
    start = entry[1];
    source = (start - D_800CFB40[0]) + (u32)bin_4A15A0_ROM_START;
    new_var = entry[2] - start;
    length = new_var;
    dest_ptr = &D_8013776C;
    *dest_ptr = (u32)ACTOR_LIST_DEST;
    D_80137770 = (u32)ACTOR_LIST_DEST + length;
    DMA_ReadSync((u8*)source, (u8*)*dest_ptr, length);
}

void func_800265FC(u16 index) {
    u32 start;
    u32 source;
    s32 new_var;
    u32* entry;

    entry = &D_800CEED0[index * 3];
    start = entry[1];
    source = (start - D_800CEED0[0]) + (u32)map_collision_4A1FC0_ROM_START;
    new_var = entry[2] - start;
    D_80137774 = (u32)COLLISION_DEST;
    DMA_ReadSync((u8*)source, RLE_SCRATCH_LOW, new_var);
    D_80137778 = (u32)COLLISION_DEST + Trouble_RLE_Type1(RLE_SCRATCH_LOW, COLLISION_DEST);
}

void func_80026694(u16 index) {
    u32 start;
    u32* entry;

    entry = &D_800CEED0[index * 3];
    start = entry[2];
    DMA_ReadSync((u8*)((start - D_800CEED0[0]) + (u32)map_collision_4A1FC0_ROM_START), COLLISION_SCRATCH_ALT, entry[3] - start);
    Trouble_RLE_Type1(COLLISION_SCRATCH_ALT, RLE_SCRATCH_LOW);
}

void func_8002670C(u16 index) {
    u32 start;
    u32 length;
    u32 source;
    s32 new_var;
    u32* dest_ptr;
    u32* entry;

    entry = &D_800CF2F4[index * 2];
    start = entry[1];
    source = (start - D_800CF2F4[0]) + (u32)map_collision_4A1FC0_ROM_START;
    new_var = entry[2] - start;
    length = new_var;
    dest_ptr = &D_8013777C;
    *dest_ptr = (u32)STAGE_TABLE0_DEST;
    D_80137780 = (u32)STAGE_TABLE0_DEST + length;
    DMA_ReadSync((u8*)source, (u8*)*dest_ptr, length);
}

void func_80026784(u16 index) {
    u32 start;
    u32 length;
    u32 source;
    s32 new_var;
    u32* dest_ptr;
    u32* entry;

    entry = &D_800CF5B8[index * 2];
    start = entry[1];
    source = (start - D_800CF5B8[0]) + (u32)map_collision_4A1FC0_ROM_START;
    new_var = entry[2] - start;
    length = new_var;
    dest_ptr = &D_80137784;
    *dest_ptr = (u32)STAGE_TABLE1_DEST;
    D_80137788 = (u32)STAGE_TABLE1_DEST + length;
    DMA_ReadSync((u8*)source, (u8*)*dest_ptr, length);
}

void func_800267FC(u16 index) {
    u32 start;
    u32 length;
    u32 source;
    s32 new_var;
    u32* dest_ptr;
    u32* entry;

    entry = &D_800CF87C[index * 2];
    start = entry[1];
    source = (start - D_800CF87C[0]) + (u32)map_collision_4A1FC0_ROM_START;
    new_var = entry[2] - start;
    length = new_var;
    dest_ptr = &D_8013778C;
    *dest_ptr = (u32)STAGE_TABLE2_DEST;
    D_80137790 = (u32)STAGE_TABLE2_DEST + length;
    DMA_ReadSync((u8*)source, (u8*)*dest_ptr, length);
}

void func_80026874(u16 index) {
    s32 start;
    u32 new_var;

    D_8013770C = D_80137710 = (u32)ASSET_DEST0;

    new_var = D_800CFE04[index * 12 + 0];
    start = new_var;
    if (start != 0) {
        start = (start - (s32)D_1000000) + bin_248F40_ROM_START;
        DMA_ReadSync(start, RLE_SCRATCH_HIGH, D_800CFE04[index * 12 + 1] - D_800CFE04[index * 12 + 0]);
        D_80137710 = (u32)ASSET_DEST0 + Trouble_RLE_Type1(RLE_SCRATCH_HIGH, ASSET_DEST0);
        D_801376D0 = D_800D0E84[index * 6];
    }
}

void func_8002694C(u16 index) {
    s32 start;
    u32 new_var;

    D_801376EC = (D_801376F0 = (u32)ASSET_DEST1);
    new_var = D_800CFE04[index * 12 + 2];
    start = new_var;
    if (start != 0) {
        start = (start - (s32)D_1000000) + bin_248F40_ROM_START;
        DMA_ReadSync(start, RLE_SCRATCH_HIGH, D_800CFE04[index * 12 + 3] - D_800CFE04[index * 12 + 2]);
        D_801376F0 = (u32)ASSET_DEST1 + Trouble_RLE_Type1(RLE_SCRATCH_HIGH, ASSET_DEST1);
        D_801376C0 = D_800D0E88[index * 6];
    }
}

// https://decomp.me/scratch/PZ6td
#ifdef NON_MATCHING
void func_80026A18(u16 index) {
    D_800CBF54 = 0;
    D_801376F4 = D_801376F8 = (u32)ASSET_DEST2;
    if (D_800CFE04[index][4] != 0) {
        if (D_800CFE04[index][4] == (u32)D_10CE330) {
            D_800CBF54 = 1;
        }
        DMA_ReadSync((D_800CFE04[index][4] - (u32)D_1000000) + bin_248F40_ROM_START, RLE_SCRATCH_HIGH, D_800CFE04[index][5] - D_800CFE04[index][4]);
        D_801376F8 = (u32)ASSET_DEST2 + Trouble_RLE_Type1(RLE_SCRATCH_HIGH, ASSET_DEST2);
        D_801376C4 = D_800D0E8C[index * 6];
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026A18.s")
#endif

void func_80026B04(u16 index) {
    s32 start;
    u32 new_var;

    D_801376FC = D_80137700 = (u32)ASSET_DEST3;
    new_var = D_800CFE04[index * 12 + 6];
    start = new_var;
    if (start != 0) {
        start = (start - (s32)D_1000000) + bin_248F40_ROM_START;
        DMA_ReadSync(start, RLE_SCRATCH_HIGH, D_800CFE04[index * 12 + 7] - D_800CFE04[index * 12 + 6]);
        D_80137700 = (u32)ASSET_DEST3 + Trouble_RLE_Type1(RLE_SCRATCH_HIGH, ASSET_DEST3);
        D_801376C8 = D_800D0E90[index * 6];
    }
}

void func_80026BD0(u16 index) {
    s32 start;
    u32 new_var;

    D_80137704 = D_80137708 = (u32)ASSET_DEST4;
    new_var = D_800CFE04[index * 12 + 8];
    start = new_var;
    if (start != 0) {
        start = (start - (s32)D_1000000) + bin_248F40_ROM_START;
        DMA_ReadSync(start, RLE_SCRATCH_HIGH, D_800CFE04[index * 12 + 9] - D_800CFE04[index * 12 + 8]);
        D_80137708 = (u32)ASSET_DEST4 + Trouble_RLE_Type1(RLE_SCRATCH_HIGH, ASSET_DEST4);
        D_801376CC = D_800D0E94[index * 6];
    }
}

#ifdef NON_MATCHING
// https://decomp.me/scratch/HqxM1
void func_80026C9C(u16 index) {
    s32 unused[2];

    D_800CBF50 = 0;
    D_80137714 = D_80137718 = (u32)ASSET_DEST5;
    if (D_800CFE04[index * 12 + 10] != 0) {
        if ((u32)&D_119EB70 == D_800CFE04[index * 12 + 10]) {
            D_800CBF50 = 1;
        }
        DMA_ReadSync(((D_800CFE04[index * 12 + 10] - (u32)D_1000000) + (u32)bin_248F40_ROM_START), RLE_SCRATCH_HIGH, D_800CFE04[index * 12 + 11] - D_800CFE04[index * 12 + 10]);
        D_80137718 = (u32)ASSET_DEST5 + Trouble_RLE_Type1(RLE_SCRATCH_HIGH, ASSET_DEST5);
        D_801376D4 = D_800D0E98[index * 6];
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026C9C.s")
#endif

void func_80026D88(u16 index) {
    s32 start;
    u32 new_var;

    D_8013770C = D_80137710 = (u32)ASSET_DEST0;
    new_var = D_800CFE04[index * 12 + 0];
    start = new_var;
    if (start != 0) {
        start = (start - (s32)D_1000000) + bin_248F40_ROM_START;
        DMA_ReadSync(start, RLE_SCRATCH_LOW, D_800CFE04[index * 12 + 1] - D_800CFE04[index * 12 + 0]);
        D_80137710 = (u32)ASSET_DEST0 + Trouble_RLE_Type1(RLE_SCRATCH_LOW, ASSET_DEST0);
        D_801376D0 = D_800D0E84[index * 6];
    }
}

void func_80026E60(u16 index) {
    s32 start;
    u32 new_var;

    D_801376EC = D_801376F0 = (u32)ASSET_DEST1;
    new_var = D_800CFE04[index * 12 + 2];
    start = new_var;
    if (start != 0) {
        start = (start - (s32)D_1000000) + bin_248F40_ROM_START;
        DMA_ReadSync(start, RLE_SCRATCH_LOW, D_800CFE04[index * 12 + 3] - D_800CFE04[index * 12 + 2]);
        D_801376F0 = (u32)ASSET_DEST1 + Trouble_RLE_Type1(RLE_SCRATCH_LOW, ASSET_DEST1);
        D_801376C0 = D_800D0E88[index * 6];
    }
}

#ifdef NON_MATCHING
void func_80026F2C(u16 index) {
    u32 start;
    s32 unused[2];

    D_800CBF54 = 0;
    D_801376F4 = D_801376F8 = (u32)ASSET_DEST2;
    start = D_800CFE04[index * 12 + 4];
    if (start != 0) {
        if ((u32)&D_10CE330 == start) {
            D_800CBF54 = 1;
        }
        DMA_ReadSync((void*)((start - (u32)&D_1000000) + (u32)bin_248F40_ROM_START), RLE_SCRATCH_LOW,
                     D_800CFE04[index * 12 + 5] - start);
        D_801376F8 = (u32)ASSET_DEST2 + Trouble_RLE_Type1(RLE_SCRATCH_LOW, ASSET_DEST2);
        D_801376C4 = D_800D0E8C[index * 6];
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_80026F2C.s")
#endif

void func_80027018(u16 index) {
    s32 start;
    u32 new_var;

    D_801376FC = D_80137700 = (u32)ASSET_DEST3;
    new_var = D_800CFE04[index * 12 + 6];
    start = new_var;
    if (start != 0) {
        start = (start - (s32)D_1000000) + bin_248F40_ROM_START;
        DMA_ReadSync(start, RLE_SCRATCH_LOW, D_800CFE04[index * 12 + 7] - D_800CFE04[index * 12 + 6]);
        D_80137700 = (u32)ASSET_DEST3 + Trouble_RLE_Type1(RLE_SCRATCH_LOW, ASSET_DEST3);
        D_801376C8 = D_800D0E90[index * 6];
    }
}

void func_800270E4(u16 index) {
    s32 start;
    u32 new_var;

    D_80137704 = D_80137708 = (u32)ASSET_DEST4;
    new_var = D_800CFE04[index * 12 + 8];
    start = new_var;
    if (start != 0) {
        start = (start - (s32)D_1000000) + bin_248F40_ROM_START;
        DMA_ReadSync(start, RLE_SCRATCH_LOW, D_800CFE04[index * 12 + 9] - D_800CFE04[index * 12 + 8]);
        D_80137708 = (u32)ASSET_DEST4 + Trouble_RLE_Type1(RLE_SCRATCH_LOW, ASSET_DEST4);
        D_801376CC = D_800D0E94[index * 6];
    }
}

#ifdef NON_MATCHING
void func_800271B0(u16 index) {
    u32 start;
    s32 unused[2];

    D_800CBF50 = 0;
    D_80137714 = D_80137718 = (u32)ASSET_DEST5;
    start = D_800CFE04[index * 12 + 10];
    if (start != 0) {
        if ((u32)&D_119EB70 == start) {
            D_800CBF50 = 1;
        }
        DMA_ReadSync((void*)((start - (u32)&D_1000000) + (u32)bin_248F40_ROM_START), RLE_SCRATCH_LOW,
                     D_800CFE04[index * 12 + 11] - start);
        D_80137718 = (u32)ASSET_DEST5 + Trouble_RLE_Type1(RLE_SCRATCH_LOW, ASSET_DEST5);
        D_801376D4 = D_800D0E98[index * 6];
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/26A00/func_800271B0.s")
#endif

void func_8002729C(u16 index) {
    u32 start;

    D_8013771C = D_80137720 = (u32)ASSET_DEST6;
    ;
    if ((u32)&D_40E17F0 == D_800CEC10[index * 2]) {
    }
    else if ((u32)&D_4000000 == D_800CEC10[index * 2]) {
    }
    else {
        start = (u32)&D_11BD940;
        DMA_ReadSync((u8*)((start + (u32)bin_248F40_ROM_START) - (u32)&D_1000000), RLE_SCRATCH_LOW,
                     (u32)&D_11C1D30 - start);
        D_80137720 = (u32)ASSET_DEST6 + Trouble_RLE_Type1(RLE_SCRATCH_LOW, ASSET_DEST6);
        D_801376D8 = 0x803D7748;
    }
}
