#include "common.h"
#include "thread.h"

// likely all actually in boot

Gfx* gDListHead;
OSMesg D_8012A678[8];

OSThread sIdleThread;
OSThread sMainThread;
OSThread sRmonThread;

OSMesgQueue sDMAMesgQ;

OSMesgQueue D_8012ABC0;
OSMesgQueue D_8012ABD8;
OSMesgQueue D_8012ABF0;
OSMesgQueue D_8012AC08;
OSMesgQueue D_8012AC20;
OSMesgQueue D_8012AC38[2];

OSMesg D_8012AC68;
OSMesg D_8012AC6C;
OSMesg D_8012AC70;
OSMesg D_8012AC74;
OSMesg D_8012AC78;
OSMesg D_8012AC7C;
OSMesg D_8012AC80;
OSTask* gGFXTaskp;
OSTask gGFXTasks[2];

OSViMode* gOSViModep;
OSViMode gOSViMode;

OSContStatus D_8012AD60[4];

OSContPad gContpadArrayA[4];
OSContPad gContpadArrayB[4];
OSMesgQueue gContMesgQ;

OSMesg D_8012ADB8;

GfxData gDListTail[2];

u32 gPlayerControllerIndex;
u16 gButtonCurrent;

u16 D_801370C6;
u32 D_801370C8;
u16 D_801370CC;
u16 D_801370CE;
