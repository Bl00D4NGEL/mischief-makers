#include "common.h"
#include "actor.h"

extern s32 D_80137420;
extern u16 D_800D28E4;
extern u32 D_800D28FC;

extern s32 func_800032C4(u32 arg0);
extern s32 func_8008D0A8(u16 actor_index);
extern s32 func_8008D418(u16 actor_index);
extern s32 func_8008D480(u16 actor_index);
extern void func_8008CF60(u16 actor_index);

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008C4A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008C4A8.s")

void func_8008C4B0(u32 arg0, u32 arg1, u32 arg2) {
}

void func_8008C4C0(u32 arg0) {
}

void func_8008C4C8(u32 arg0) {
}

void func_8008C4D0(u32 arg0) {
}

void func_8008C4D8(u32 arg0) {
}

void func_8008C4E0(u16 actor_index) {
    gActors[actor_index].colorB = 0;
    gActors[actor_index].colorG = 0;
    gActors[actor_index].colorR = 0;
    D_80137420 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008C528.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008C710.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008CA90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008CC00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008CC90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008CDC4.s")

void func_8008CF10(u16 actor_index) {
    gActors[actor_index].unk_0AE = 8;
    gActors[actor_index].unk_0B0 = -0xE;
    gActors[actor_index].unk_0AA = -8;
    gActors[actor_index].unk_0AC = 8;
}

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008CF60.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008CFE4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008D0A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008D128.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008D1E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008D2B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008D320.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008D39C.s")

s32 func_8008D418(u16 actor_index) {
    if (gActors[actor_index].unk_098 & 0x40) {
        func_8008D0A8(actor_index);
        return 1;
    }

    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008D480.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008D510.s")

void func_8008D728(u16 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008D730.s")

void func_8008D958(u16 arg0) {
}

void func_8008D960(u16 actor_index) {
    if (func_8008D418(actor_index) == 0) {
        func_8008CF10(actor_index);
    }
}

void func_8008D99C(u16 actor_index) {
    if (func_8008D418(actor_index) == 0) {
        func_8008CF10(actor_index);
        if (gActors[actor_index].unk_118 == 0.0f) {
            func_8008CF60(actor_index);
        }
    }
}

s32 func_8008DA24(u16 actor_index) {
    return func_8008D480(actor_index);
}

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008DA4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008DC70.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008DCE0.s")

void func_8008DE20(u16 arg0) {
}

void func_8008DE28(u16 arg0) {
}

void func_8008DE30(u16 actor_index) {
    func_800032C4(0xB5);
    func_800032C4(0xB4);

    if (gActors[actor_index].velocityY > FIXED_UNIT(-6.5)) {
        gActors[actor_index].velocityY -= FIXED_UNIT(0.1953125);
    }

    if (gActors[actor_index].posY.whole < -0x100) {
        gActors[actor_index].state = 0x40;
    }
}

void func_8008DEBC(u16 actor_index) {
    if (D_800D28E4 > 0x60) {
        gActors[actor_index].flags = 0;
    }
    else {
        D_800D28FC |= 0x200;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008DF20.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008E1A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008E310.s")

#pragma GLOBAL_ASM("asm/nonmatchings/8D0A0/func_8008E3C0.s")
