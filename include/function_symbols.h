#ifndef FUNCTION_SYMBOLS_H
#define FUNCTION_SYMBOLS_H

#include "common.h"

extern void Sound_InitPlayers(void);
extern void Sound_SetEventMesg(void);
extern void Sound_Update(void);
extern void Sound_NextBuffer(void);
extern void Sound_StartTask(void);

extern void func_80022D10(void);

extern void LookAt_Update(void);

// func_800012F0
extern void func_80020844(void);
extern void func_800208D4(void);
extern void func_800838E0(void);

// func_8000147C
extern void func_80009940(void);
extern void func_80009BE0(void);
extern void func_80009BE8(u16*);
extern void func_8000DD6C(void);
extern void func_8000EA88(void);
extern void func_8000F290(void);
extern void func_8001FF28(void);
extern void func_80021620(void);
extern void func_80021658(void);
extern void func_80021660(void);
extern void func_8002167C(void);
extern void func_800218FC(void);
extern void func_800821B0(void);
extern void func_800822B8(void);
extern void func_80082CFC(void);
extern void func_80082E04(void);
extern void func_80082F10(void);
extern void func_80083E74(void);

#endif
