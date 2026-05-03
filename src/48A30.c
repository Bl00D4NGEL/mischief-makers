#include "common.h"
#include "actor.h"

extern s32 D_800BE5F4;

void func_80047E30(u16 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/48A30/func_80047E38.s")

#pragma GLOBAL_ASM("asm/nonmatchings/48A30/func_80047ED4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/48A30/func_80047F80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/48A30/func_80047FF8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/48A30/func_800480B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/48A30/func_80048238.s")

#pragma GLOBAL_ASM("asm/nonmatchings/48A30/func_80048320.s")

void func_800483B4(u16 actor_index) {
    if (gActors[actor_index].stateLower == 3) {
        gActors[actor_index].unk_150 = 0x258;
    }

    D_800BE5F4 = 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/48A30/func_80048408.s")

#pragma GLOBAL_ASM("asm/nonmatchings/48A30/func_800485AC.s")
