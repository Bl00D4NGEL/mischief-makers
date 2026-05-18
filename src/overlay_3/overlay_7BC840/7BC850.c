#include "common.h"
#include "actor.h"
#include "function_symbols.h"

extern s16 D_800BE5D8;
extern s16 D_800BE5DC;
extern f32 D_801B4720_7C0670;
extern u8 D_801B4B60_7C0AB0;
extern u8 D_801B4B70_7C0AC0;

extern void func_8003EEC0(f32 arg0, s16 arg1, s16 arg2, s16 arg3);
extern void func_800339BC(s32 arg0, s32 arg1, s32 arg2, u16 arg3);
extern s32 func_800036C8(u32 arg0, u16 actor_index);

void func_801B0900_7BC850(u16 actor_index) {
    gActors[actor_index].flags &= 0xFFFFF7FF;
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B0944_7BC894.s")

void func_801B0A10_7BC960(u16 actor_index) {
    func_8003EEC0(
        D_801B4720_7C0670,
        gActors[actor_index].posX.whole,
        gActors[actor_index].posY.whole,
        gActors[actor_index].posZ.whole + 4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B0A7C_7BC9CC.s")

void func_801B0BA0_7BCAF0(u16 actor_index) {
    gActors[actor_index].velocityX = Math_ApproachS32(gActors[actor_index].velocityX, 0, 0x1800);
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B0C00_7BCB50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B0CBC_7BCC0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B0D80_7BCCD0.s")

void func_801B0E58_7BCDA8(u16 actor_index, s32 x, s32 y, u16 arg3) {
    func_800339BC(
        gActors[actor_index].posX.raw + x,
        gActors[actor_index].posY.raw + y,
        gActors[actor_index].posZ.raw,
        arg3);
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B0ECC_7BCE1C.s")

void func_801B11A0_7BD0F0(u16 actor_index) {
    gActors[actor_index].unk_0AA = -8;
    gActors[actor_index].unk_0AC = 8;
    gActors[actor_index].unk_0AE = 8;
    gActors[actor_index].unk_0B0 = -4;
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B11F0_7BD140.s")

s32 func_801B1390_7BD2E0(u16 actor_index) {
    if (gActors[actor_index].posX.whole < gActors[0].posX.whole) {
        if (gActors[actor_index].flags & 0x20) {
            gActors[actor_index].state = 0x22;
            return 1;
        }
    }
    else if (!(gActors[actor_index].flags & 0x20)) {
        gActors[actor_index].state = 0x22;
        return 2;
    }

    return 0;
}

s32 func_801B141C_7BD36C(u16 actor_index) {
    if (gActors[actor_index].posX.whole < gActors[0].posX.whole) {
        if (gActors[actor_index].flags & 0x20) {
            gActors[actor_index].flags &= 0xFFFFFFDF;
            return 1;
        }
    }
    else if (!(gActors[actor_index].flags & 0x20)) {
        gActors[actor_index].flags |= 0x20;
        return 2;
    }

    return 0;
}

void func_801B14AC_7BD3FC(s32 arg0) {
    if ((D_801B4B60_7C0AB0 & 3) == 0) {
        if (D_800BE5D8 >= 0x209) {
            if (D_800BE5DC < 0x170) {
                if (D_801B4B70_7C0AC0 == 0) {
                    D_801B4B70_7C0AC0 = 1;
                }
                D_801B4B60_7C0AB0 |= 8;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B1510_7BD460.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B1570_7BD4C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B1994_7BD8E4.s")

void func_801B1B34_7BDA84(u16 arg0, u16 arg1) {
    gActors[0x57].state = arg0;
    gActors[0x59].state = arg0;
    gActors[0x58].state = arg0;
    gActors[0x5A].state = arg0;
    gActors[0x57].unk_174 = arg1;
    gActors[0x59].unk_174 = arg1;
    gActors[0x58].unk_174 = arg1;
    gActors[0x5A].unk_174 = arg1;
}

void func_801B1B6C_7BDABC(u16 arg0, u16 arg1) {
    gActors[0x5B].state = arg0;
    gActors[0x5C].state = arg0;
    gActors[0x5D].state = arg0;
    gActors[0x5E].state = arg0;
    gActors[0x5B].unk_174 = arg1;
    gActors[0x5C].unk_174 = arg1;
    gActors[0x5D].unk_174 = arg1;
    gActors[0x5E].unk_174 = arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B1BA4_7BDAF4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B3068_7BEFB8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B31B8_7BF108.s")

f32 func_801B3480_7BF3D0(u16 actor_index) {
    return -40.0f - (((1.0f - gActors[actor_index].scaleX) * 40.0f) / 2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B34E4_7BF434.s")

void func_801B35E0_7BF530(u16 actor_index) {
    if (gActors[actor_index].posX.whole > 0) {
        gActors[actor_index].flags |= 0x20;
    }
    else {
        gActors[actor_index].flags &= 0xFFFFFFDF;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B3648_7BF598.s")

void func_801B3F18_7BFE68(u16 actor_index) {
    u16 temp = gActors[actor_index].unk_0D8 & 7;

    if (gActors[actor_index].state == 0) {
        gActors[actor_index].state++;
        gActors[actor_index].flags = 3;
        if (temp == 0) {
            gActors[actor_index].unk_084 = 0x81C;
            gActors[actor_index].posZ.whole = -0xC0;
        }
    }
}

void func_801B3F84_7BFED4(u16 arg0) {
    u16 actor_index;

    for (actor_index = 0x57; actor_index < 0x5F; actor_index++) {
        gActors[actor_index].unk_174 = arg0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_3/overlay_7BC840/7BC850/func_801B3FC8_7BFF18.s")
