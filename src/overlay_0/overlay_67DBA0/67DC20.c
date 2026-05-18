#include "common.h"
#include "actor.h"
#include "function_symbols.h"

extern u8 D_801376BC[];
extern u8 D_801376B4[];
extern u8 D_801376A8[];
extern u8 D_801376AC[];
extern u8 D_801376B0[];
extern u16 D_800BE4E0;
extern s32 D_800BE554;
extern u32 D_8019B200_686D20[];
extern u32 D_8019C054_687B74[];
extern u32 D_8019C8D8_6883F8[];
extern u32 D_8019D784_6892A4[];

void func_801939CC_67F4EC(u16 actor_index);
void func_80193A4C_67F56C(u16 actor_index);
void func_80193ACC_67F5EC(u16 actor_index);
void func_80193B4C_67F66C(u16 actor_index);
void func_80193C14_67F734(u16 actor_index);
void func_800816AC(u16 actor_index, void* arg1, s32 arg2);
void func_80081720(u16 actor_index, void* arg1, s32 arg2);

void func_80192100_67DC20(s32 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80192108_67DC28.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019219C_67DCBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019237C_67DE9C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_801923B0_67DED0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_801926CC_67E1EC.s")

void func_801927DC_67E2FC(s32 arg0) {
    s32 actor_index = gActors[0x46].unk_180;

    if (actor_index != 0) {
        actor_index = (u16)actor_index;
        gActors[actor_index].flags = 0;
    }
    gActors[0x46].unk_180 = 0;
}

void func_80192820_67E340(s32 arg0) {
    s32 actor_index = gActors[0x46].unk_180;

    if (actor_index != 0) {
        actor_index = (u16)actor_index;
        if (gActors[actor_index].flags & 0x20) {
            gActors[actor_index].unk_160 = (-(gActors[0x37].unk_150) << 0x10) + 0x2000000;
            return;
        }
        gActors[actor_index].unk_160 = (gActors[0x37].unk_150 << 0x10) + 0x2000000;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_801928A8_67E3C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_801929C0_67E4E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80192C4C_67E76C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80192E68_67E988.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80192FE4_67EB04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_801934F0_67F010.s")

void func_80193594_67F0B4(void) {
}

void func_8019359C_67F0BC(void) {
}

void func_801935A4_67F0C4(void) {
    gActors[0x61].actorType = 0x607;
    func_8001E2D0(0x61);
    gActors[0x61].flags = 0x2002;
    gActors[0x61].unk_0AE = 0x10;
    gActors[0x61].unk_0B0 = -0x10;
    gActors[0x61].unk_0AA = -0x100;
    gActors[0x61].unk_0AC = 0x100;
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80193600_67F120.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80193694_67F1B4.s")

void func_80193728_67F248(u16 actor_index) {
    gActors[actor_index].actorType = 0x1D;
    func_8001E2D0(actor_index);
    gActors[actor_index].unk_094 |= 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80193790_67F2B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_801939CC_67F4EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80193A4C_67F56C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80193ACC_67F5EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80193B4C_67F66C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80193C14_67F734.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80193CC4_67F7E4.s")

void func_80193DF0_67F910(s32 arg0, u16 index) {
    u32* ptr = &D_8019D784_6892A4[index * 5];

    func_80081790(0x4D, (void*)ptr[0]);
    func_80081790(0x3F, (void*)ptr[1]);
    func_80081790(0x31, (void*)ptr[2]);
    func_80081790(0x38, (void*)ptr[3]);
    func_80081790(0x5C, (void*)ptr[4]);
}

void func_80193E6C_67F98C(s32 arg0) {
    func_800816AC(0x31, D_8019B200_686D20, arg0);
    func_800816AC(0x38, D_8019B200_686D20, arg0);
    func_800816AC(0x3F, D_8019C054_687B74, arg0);
    func_800816AC(0x4D, D_8019C8D8_6883F8, arg0);
}

void func_80193ED8_67F9F8(s32 arg0) {
    func_80081720(0x31, D_8019B200_686D20, arg0);
    func_80081720(0x38, D_8019B200_686D20, arg0);
    func_80081720(0x3F, D_8019C054_687B74, arg0);
    func_80081720(0x4D, D_8019C8D8_6883F8, arg0);
}

void func_80193F44_67FA64(s32 arg0, u8 arg1) {
    gActors[0x3F].unk_094 |= 0x10;
    gActors[0x3F].colorR = arg1;
    gActors[0x3F].colorG = arg1;
    gActors[0x3F].colorB = arg1;
}

void func_80193F70_67FA90(s32 arg0, u8 arg1) {
    gActors[0x3F].unk_094 &= 0xFFEF;
    gActors[0x3F].colorR = arg1;
    gActors[0x3F].colorG = arg1;
    gActors[0x3F].colorB = arg1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80193F9C_67FABC.s")

void func_80194818_680338(void) {
    s32 step;

    step = 0xC000;
    if (D_800BE4E0 & 0x20) {
        step = 0x2000;
    }

    gActors[0x45].unk_180 = Math_ApproachS32(gActors[0x45].unk_180, step, 0x800);
    gActors[0x40].unk_180 = Math_ApproachS32(gActors[0x40].unk_180, 0, gActors[0x45].unk_180);
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019487C_68039C.s")

void func_80194934_680454(s32 arg0) {
    if (gActors[0x3F].posZ.raw >= -0x27FFFF) {
        gActors[0x47].unk_180 = Math_ApproachS32(gActors[0x47].unk_180, 0, 0x6000);
    }
    else {
        gActors[0x47].unk_180 = Math_ApproachS32(gActors[0x47].unk_180, 0x2C000, 0x6000);
    }
    gActors[0x3F].posZ.raw += gActors[0x47].unk_180;
}

void func_801949B8_6804D8(s32 arg0) {
    if (gActors[0x3F].posZ.raw < -0x780000) {
        gActors[0x47].unk_180 = Math_ApproachS32(gActors[0x47].unk_180, 0, 0x6000);
    }
    else {
        gActors[0x47].unk_180 = Math_ApproachS32(gActors[0x47].unk_180, -0x2A000, 0x6000);
    }
    gActors[0x3F].posZ.raw += gActors[0x47].unk_180;
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80194A38_680558.s")

void func_80194AC8_6805E8(void) {
    gActors[0x60].colorA = Math_ApproachS32(gActors[0x60].colorA, 0, 8);
    if (gActors[0x60].colorA == 0) {
        gActors[0x60].flags = 0;
    }
}

void func_80194B10_680630(void) {
    D_801376BC[0] = 0;
    D_801376B4[0] = 0x80;
    D_801376A8[0] = 0;
    D_801376AC[0] = 0;
    D_801376B0[0] = 0;
    D_801376BC[2] = 0;
    D_801376B4[2] = 0x80;
    D_801376A8[2] = 0;
    D_801376AC[2] = 0;
    D_801376B0[2] = 0;
}

void func_80194B68_680688(void) {
    D_801376BC[0] = 1;
    D_801376BC[2] = 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80194B80_6806A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80194D3C_68085C.s")

void func_80194E64_680984(u16 actor_index, u16 arg1) {
    gActors[actor_index].unk_110 =
        (f32)(arg1 + (gActors[actor_index].health / 0x1E) - (func_8000178C() & 0x2F) + 0x14);
}

void func_80194EE4_680A04(void) {
    if (D_800BE4E0 & 0x80) {
        D_800BE554 = Math_ApproachS32(D_800BE554, 0x19C0000, 0x1000);
    }
    else {
        D_800BE554 = Math_ApproachS32(D_800BE554, 0x1A20000, 0x1000);
    }
}

void func_80194F48_680A68(void) {
    if (gActors[0x37].flags & 0x200) {
        gActors[0x37].unk_094 &= 0xFFEF;
        Actor_SetColorRgb(0x37, 0x2F);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80194F8C_680AAC.s")

void func_80197EFC_683A1C(s32 actor_index) {
    func_801939CC_67F4EC(actor_index + 2);
    func_80193A4C_67F56C(actor_index + 4);
    func_80193ACC_67F5EC(actor_index + 6);
}

void func_80197F58_683A78(s32 actor_index) {
    func_80193B4C_67F66C(actor_index - 0x1D);
    func_80193C14_67F734(actor_index - 0x1D);
}

void func_80197F90_683AB0(s32 arg0) {
}

void func_80197F98_683AB8(s32 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80197FA0_683AC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019809C_683BBC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80198708_684228.s")

void func_8019882C_68434C(void) {
    func_800284B8(0x67, 0x68);
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80198850_684370.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019893C_68445C.s")

void func_80198ED8_6849F8(s32 arg0) {
}

void func_80198EE0_684A00(s32 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80198EE8_684A08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80198F70_684A90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80199094_684BB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019911C_684C3C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_801992AC_684DCC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80199DA8_6858C8.s")

void func_80199F54_685A74(s32 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_80199F5C_685A7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019A3B8_685ED8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019A604_686124.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019A6AC_6861CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019A888_6863A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019AB04_686624.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019AD40_686860.s")

#pragma GLOBAL_ASM("asm/nonmatchings/overlay_0/overlay_67DBA0/67DC20/func_8019AEE4_686A04.s")
