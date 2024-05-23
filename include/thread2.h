#ifndef GAMETHREAD2_H
#define GAMETHREAD2_H

#include <ultra64.h>
#include "inttypes.h"
#include "common_structs.h"

// not actually thread related, just in the same bss section

extern Gfx* gDListHead;
extern OSMesg D_8012A678[8];

extern OSMesgQueue sDMAMesgQ;

extern OSMesgQueue D_8012ABC0;
extern OSMesgQueue D_8012ABD8;
extern OSMesgQueue D_8012ABF0;
extern OSMesgQueue D_8012AC08;
extern OSMesgQueue D_8012AC20;
extern OSMesgQueue D_8012AC38[2];

extern OSMesg D_8012AC68;
extern OSMesg D_8012AC6C;
extern OSMesg D_8012AC70;
extern OSMesg D_8012AC74;
extern OSMesg D_8012AC78;
extern OSMesg D_8012AC7C;
extern OSMesg D_8012AC80;
extern OSTask* gGFXTaskp;
extern OSTask gGFXTasks[2];

extern OSViMode* gOSViModep;
extern OSViMode gOSViMode;

extern OSContStatus D_8012AD60[4];

extern OSContPad gContpadArrayA[4];
extern OSContPad gContpadArrayB[4];
extern OSMesgQueue gContMesgQ;

extern OSMesg D_8012ADB8;

extern GfxData gDListTail[2];

extern u32 gPlayerControllerIndex;
extern u16 gButtonCurrent;

extern u16 D_801370C6;
extern u32 D_801370C8;
extern u16 D_801370CC;
extern u16 D_801370CE;

#endif

