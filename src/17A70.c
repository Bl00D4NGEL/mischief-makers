#include "common.h"
#include "actor.h"

extern ActorFunc D_800E5AC0[];
extern ActorFunc D_80192000[];
extern ActorFunc D_8019B000[];
extern ActorFunc D_801A6800[];
extern ActorFunc D_801B0800[];

void func_80016E70(u16 actor_index) {
    u8 index = D_800EF510[actor_index].unk_0D2 & 0xFFFF;

    switch (D_800EF510[actor_index].unk_0D2 >> 8) {
        case 1:
            D_801B0800[index](actor_index);
            break;

        case 2:
            D_8019B000[index](actor_index);
            break;

        case 3:
            D_8019B000[index](actor_index);
            break;

        case 4:
            D_801B0800[index](actor_index);
            break;

        case 5:
            D_801B0800[index](actor_index);
            break;

        case 6:
            D_80192000[index](actor_index);
            break;

        case 7:
            D_801B0800[index](actor_index);
            break;

        case 8:
            D_800E5AC0[index](actor_index);
            break;

        case 9:
            D_8019B000[index](actor_index);
            break;

        case 10:
            D_8019B000[index](actor_index);
            break;

        case 11:
            D_8019B000[index](actor_index);
            break;

        case 12:
            D_8019B000[index](actor_index);
            break;

        case 13:
            D_8019B000[index](actor_index);
            break;

        case 14:
            D_801B0800[index](actor_index);
            break;

        case 15:
            D_80192000[index](actor_index);
            break;

        case 16:
            D_80192000[index](actor_index);
            break;

        case 17:
            D_8019B000[index](actor_index);
            break;

        case 18:
            D_8019B000[index](actor_index);
            break;

        case 19:
            D_8019B000[index](actor_index);
            break;

        case 20:
            D_80192000[index](actor_index);
            break;

        case 21:
            D_80192000[index](actor_index);
            break;

        case 22:
            D_80192000[index](actor_index);
            break;

        case 23:
            D_801A6800[index](actor_index);
            break;

        case 24:
            D_801A6800[index](actor_index);
            break;

        case 25:
            D_80192000[index](actor_index);
            break;

        case 26:
            D_801A6800[index](actor_index);
            break;

        case 27:
            D_801B0800[index](actor_index);
            break;

        case 28:
            D_801A6800[index](actor_index);
            break;

        case 29:
            D_801A6800[index](actor_index);
            break;

        case 30:
            D_801A6800[index](actor_index);
            break;

        case 31:
            D_801A6800[index](actor_index);
            break;

        case 32:
            D_801A6800[index](actor_index);
            break;

        case 33:
            D_80192000[index](actor_index);
            break;

        case 34:
            D_80192000[index](actor_index);
            break;

        case 35:
            D_80192000[index](actor_index);
            break;

        case 36:
            D_80192000[index](actor_index);
            break;

        case 37:
            D_801A6800[index](actor_index);
            break;

        case 38:
            D_80192000[index](actor_index);
            break;

        case 39:
            D_801B0800[index](actor_index);
            break;

        case 40:
            D_801B0800[index](actor_index);
            break;

        case 41:
            D_801B0800[index](actor_index);
            break;

        case 42:
            D_801B0800[index](actor_index);
            break;

        case 43:
            D_80192000[index](actor_index);
            break;

        case 44:
            D_80192000[index](actor_index);
            break;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001751C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80017680.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_800176F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80017770.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/GameState_Intro.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80017F08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80017FE8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001809C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_800180FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/GameState_TitleScreen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/GameState_DebugSoundTest.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80018E00.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/GameState_DebugStageSelect.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80019520.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80019688.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_800197A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001983C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_800198B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_800199DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80019A80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80019E48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80019EC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80019F04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_80019FB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001A15C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001A254.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001A584.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001A758.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001A7E0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001A838.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001A890.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001A96C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001ACA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001B004.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001B02C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001B078.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001B1A0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001B1F8.s")

void func_8001B23C(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001B244.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001B2F4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001B3D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/GameState_Transition.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001C7A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001C7F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001C834.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001C8B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001C97C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001CA68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001CAA8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001CB6C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001CC34.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001CC8C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001CD30.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001CE04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001CF14.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001CFDC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001D040.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001D0A4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001D240.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001D5B8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/func_8001D60C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/17A70/GameState_Records.s")
