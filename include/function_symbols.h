#ifndef FUNCTION_SYMBOLS_H
#define FUNCTION_SYMBOLS_H

#include "common.h"

extern void Sound_InitPlayers(void);
extern void Sound_SetEventMesg(void);
extern void Sound_Update(void);
extern void Sound_NextBuffer();
extern void Sound_StartTask(void);

extern void func_80022D10(void);

extern void LookAt_Update(void);

#endif
