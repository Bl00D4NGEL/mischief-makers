#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

typedef struct {
    /* 0x0000 */ u8 unk_0x00[0x180];
    /* 0x0180 */ Gfx dlist[3072];
} GfxData; /* sizeof = 0x6180 */

#endif
