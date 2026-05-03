#include "common.h"
#include "actor.h"

typedef struct {
    /* 0x00 */ u8 unk_000[0x84];
    /* 0x84 */ s16 unk_084;
    /* 0x86 */ u8 unk_086[0x2];
    /* 0x88 */ s16 unk_088;
    /* 0x8A */ u8 unk_08A[0x6];
} UnkStruct_801069E0; /* size = 0x90 */

extern OSMesgQueue D_801377B8;
extern OSMesgQueue D_801377D0;
extern OSMesg D_801378C0;
extern OSIoMesg D_801378C8;
extern OSTask* D_8016E6F0;
extern u8 D_8011CDF0[];
extern u8 D_8011CF18[];
extern UnkStruct_801069E0 D_801069E0[];
extern u8 D_800EF4D0;
extern u8 D_800EF4F0[];
extern u16 D_800EF508[];
extern ALSeqPlayer* D_8016DFE4;
extern ALSeqPlayer* D_8016E1D8[];

extern s32 func_80003020(u32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_80003A38(void);
void func_80003A64(void);

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_800017D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_80001988.s")

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_80001A80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/music/Sound_InitPlayers.s")

void Sound_SetEventMesg(void) {
    osCreateMesgQueue(&D_801377D0, &D_801378C0, 1);
    osSetEventMesg(OS_EVENT_SP, &D_801377D0, NULL);
    osSendMesg(&D_801377D0, NULL, OS_MESG_BLOCK);
}

#pragma GLOBAL_ASM("asm/nonmatchings/music/Sound_Update.s")

#pragma GLOBAL_ASM("asm/nonmatchings/music/Sound_NextBuffer.s")

void Sound_StartTask(void) {
    osWritebackDCacheAll();
    osSpTaskLoad(D_8016E6F0);
    osSpTaskStartGo(D_8016E6F0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_80002A2C.s")

void func_80002AE0(u32 arg0, void* arg1, u32 arg2) {
    osWritebackDCacheAll();
    osPiStartDma(&D_801378C8, 0, 0, arg0, arg1, arg2, &D_801377B8);
    osRecvMesg(&D_801377B8, NULL, OS_MESG_BLOCK);
}

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_80002B50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_80002F48.s")

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_80003020.s")

s32 func_800032C4(u32 arg0) {
    u8 index;

    for (index = 0; index < 4; index++) {
        if (D_800EF4F0[index] && (arg0 == D_800EF508[index])) {
            alSeqpStop(D_8016E1D8[index]);
            return index;
        }
    }

    return -1;
}

s32 func_8000334C(u32 arg0) {
    return func_80003020(arg0, -1, -1, 0x81, 0xFF, 0);
}

s32 func_80003380(u32 arg0) {
    return func_80003020(arg0, -1, -1, 0x91, 0xFF, 0);
}

s32 func_800033B4(u32 arg0, s16 arg1) {
    return func_80003020(arg0, arg1, -1, 0x81, 0xFF, 0);
}

s32 func_800033F0(u32 arg0, s8 arg1) {
    return func_80003020(arg0, -1, arg1, 0x81, 0xFF, 0);
}

s32 func_80003430(u32 arg0, s16 arg1, s8 arg2) {
    return func_80003020(arg0, arg1, arg2, 0x81, 0xFF, 0);
}

s32 func_80003474(u32 arg0, s16 arg1, s8 arg2) {
    return func_80003020(arg0, arg1, arg2, 0x91, 0xFF, 0);
}

s32 func_800034B8(u32 arg0, s16 arg1, s8 arg2) {
    return func_80003020(arg0, arg1, arg2, 0x92, 0xFF, 0);
}

s32 func_800034FC(u32 arg0, s16 arg1, s8 arg2) {
    return func_80003020(arg0, arg1, arg2, 0x93, 0xFF, 0);
}

void func_80003540(s16 arg0, s16 arg1, s8* arg2, s16* arg3) {
    s32 temp_v0;
    s32 temp_v1;
    s16 temp;

    if (arg0 < -0x80) {
        *arg2 = 0;
    }
    else if (arg0 >= 0x80) {
        *arg2 = 0x7F;
    }
    else {
        *arg2 = (arg0 / 2) + 0x40;
    }

    if (arg0 > 0) {
        temp_v0 = arg0;
    }
    else {
        temp_v0 = -arg0;
    }

    if (arg1 > 0) {
        temp_v1 = arg1;
    }
    else {
        temp_v1 = -arg1;
    }

    temp = temp_v1 + temp_v0;
    if (temp < 0x100) {
        *arg3 = 0x100;
    }
    else if (temp < 0x200) {
        *arg3 = 0x200 - temp;
    }
    else {
        *arg3 = 0;
    }
}

s32 func_800035F8(u32 arg0, u16 actor_index) {
    s8 temp_a;
    s16 temp_b;

    if ((gActors[actor_index].posX.whole < -0x90) || (gActors[actor_index].posX.whole >= 0x90)) {
        return -1;
    }

    if ((gActors[actor_index].posY.whole < -0x60) || (gActors[actor_index].posY.whole >= 0x60)) {
        return -1;
    }

    func_80003540(gActors[actor_index].posX.whole, gActors[actor_index].posY.whole, &temp_a, &temp_b);

    if (temp_b < 0x80) {
        return -1;
    }
    else {
        return func_80003020(arg0, temp_b, temp_a, 0x81, 0xFF, 0);
    }
}

s32 func_800036C8(u32 arg0, u16 actor_index) {
    s8 temp_a;
    s16 temp_b;

    if ((gActors[actor_index].posX.whole < -0x17F) || (gActors[actor_index].posX.whole >= 0x180)) {
        return -1;
    }

    func_80003540(gActors[actor_index].posX.whole, gActors[actor_index].posY.whole, &temp_a, &temp_b);

    if (temp_b < 0x80) {
        return -1;
    }
    else {
        return func_80003020(arg0, temp_b, temp_a, 0x81, 0xFF, 0);
    }
}

s32 func_80003778(u32 arg0, u16 actor_index) {
    s8 temp_a;
    s16 temp_b;

    if ((gActors[actor_index].posX.whole < -0x17F) || (gActors[actor_index].posX.whole >= 0x180)) {
        return -1;
    }

    func_80003540(gActors[actor_index].posX.whole, gActors[actor_index].posY.whole, &temp_a, &temp_b);

    if (temp_b < 0x80) {
        return -1;
    }
    else {
        return func_80003020(arg0, temp_b, temp_a, 0x91, 0xFF, 0);
    }
}

s32 func_80003828(u32 arg0, u16 arg1) {
    s8 temp_a;
    s16 temp_b;

    if ((D_801069E0[arg1].unk_084 < -0x17F) || (D_801069E0[arg1].unk_084 >= 0x180)) {
        return -1;
    }

    func_80003540(D_801069E0[arg1].unk_084, D_801069E0[arg1].unk_088, &temp_a, &temp_b);

    if (temp_b < 0x80) {
        return -1;
    }
    else {
        return func_80003020(arg0, temp_b, temp_a, 0x81, 0xFF, 0);
    }
}

s32 func_800038C8(u32 arg0, u16 actor_index, u16 arg2) {
    s8 temp_a;
    s16 temp_b;

    if ((gActors[actor_index].posX.whole < -0x17F) || (gActors[actor_index].posX.whole >= 0x180)) {
        return -1;
    }

    func_80003540(gActors[actor_index].posX.whole, gActors[actor_index].posY.whole, &temp_a, &temp_b);

    if (temp_b < 0x80) {
        return -1;
    }
    else {
        return func_80003020(arg0, temp_b, temp_a, 0xA1, 0xFF, arg2);
    }
}

void func_80003980(u32 arg0, u16 arg1) {
    func_80003020(arg0, -1, -1, 0xC1, arg1, 0);
}

s32 func_800039B8(u32 arg0, s32 arg1, s32 arg2) {
    s32 index;

    index = func_80003020(arg0, -1, -1, 0x89, 0xFF, 0);
    D_8011CDF0[index] = 0x7F;
    D_8011CF18[index] = 0x40;
    return index;
}

void func_80003A10(void) {
    func_80003A38();
    func_80003A64();
}

void func_80003A38(void) {
    alSeqpStop(D_8016DFE4);
    D_800EF4D0 = 0;
}

void func_80003A64(void) {
    u8 index;

    for (index = 0; index < 4; index++) {
        alSeqpStop(D_8016E1D8[index]);
        D_800EF4F0[index] = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_80003AD4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_80003D64.s")

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_80003F24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/music/func_800040A0.s")
