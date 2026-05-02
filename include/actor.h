#ifndef ACTOR_H
#define ACTOR_H

#include <PR/ultratypes.h>

typedef void (*ActorFunc)(u16 actor_index);

typedef struct {
    /* 0x000 */ u8 unk_000[0x80];
    /* 0x080 */ s32 unk_080;
    /* 0x084 */ u8 unk_084[0x4];
    /* 0x088 */ s16 unk_088;
    /* 0x08A */ u8 unk_08A[0x2];
    /* 0x08C */ s16 unk_08C;
    /* 0x08E */ u8 unk_08E[0x2];
    /* 0x090 */ s16 unk_090;
    /* 0x092 */ s16 unk_092;
    /* 0x094 */ u16 unk_094;
    /* 0x096 */ u8 unk_096[0x2];
    /* 0x098 */ s32 unk_098;
    /* 0x09C */ u8 unk_09C[0x3];
    /* 0x09F */ u8 unk_09F;
    /* 0x0A0 */ u8 unk_0A0[0x10];
    /* 0x0B0 */ u8 unk_0B0[0x4];
    /* 0x0B4 */ f32 unk_0B4;
    /* 0x0B8 */ f32 unk_0B8;
    /* 0x0BC */ u8 unk_0BC[0x14];
    /* 0x0D0 */ u16 unk_0D0;
    /* 0x0D2 */ u16 unk_0D2;
    /* 0x0D4 */ u8 unk_0D4[0x12];
    /* 0x0E6 */ s16 unk_0E6;
    /* 0x0E8 */ void* unk_0E8;
    /* 0x0EC */ s32 unk_0EC;
    /* 0x0F0 */ s32 unk_0F0;
    /* 0x0F4 */ u8 unk_0F4[0x1C];
    /* 0x110 */ f32 unk_110;
    /* 0x114 */ u8 unk_114[0x34];
    /* 0x148 */ f32 unk_148;
    /* 0x14C */ u8 unk_14C[0x4];
    /* 0x150 */ void* unk_150;
    /* 0x154 */ s32 unk_154;
    /* 0x158 */ u8 unk_158[0x20];
    /* 0x178 */ void* unk_178;
    /* 0x17C */ u8 unk_17C[0x4];
    /* 0x180 */ s32 unk_180;
    /* 0x184 */ u8 unk_184[0x14];
} Actor; /* size = 0x198 */

extern Actor D_800EF510[];

#endif
