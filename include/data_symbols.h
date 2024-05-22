#ifndef DATA_SYMBOLS_H
#define DATA_SYMBOLS_H

#include "common.h"

extern u16 gCurrentFramebufferIndex;

extern OSMesgQueue D_8012AC38[2];
extern OSMesg D_8012A678[8];

extern OSViMode* gOSViModep;
extern OSViMode gOSViMode;

extern OSTask* gGFXTaskp;
extern OSTask gGFXTasks[2];
extern GfxData gDListTail[2];
extern Gfx* gDListHead;
extern Gfx* D_800EF4F4; // "I don't think this is actually a Gfx*" - me?

#endif
