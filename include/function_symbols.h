#ifndef FUNCTION_SYMBOLS_H
#define FUNCTION_SYMBOLS_H

#include "common.h"

extern void Sound_InitPlayers(void);
extern void Sound_SetEventMesg(void);
extern void Sound_Update(void);
extern void Sound_NextBuffer(void);
extern void Sound_StartTask(void);
extern void Sound_PlayMusic(u32 sequence_id);

extern u16 func_8000178C(void);
extern void func_80003F24(u16 arg0, u16 arg1);

extern void func_80022D10(void);
extern DEFAULT_INT func_8001E2D0(u16 actor_index);
extern void func_80026E60(u16 arg0);
extern u16 func_800284B8(u16 arg0, u16 arg1);
extern void func_800285E4(u16 arg0, u16 arg1);
extern void func_80028744(void);
extern s32 func_8002981C(s32 arg0, s32 arg1, s32 arg2);
extern void func_8002AEB4(u16 actor_index, s32 arg1);
extern void func_8002B82C(void* arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void func_80042E84(
    s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8
);
extern void func_80043A68(void* arg0);
extern void func_80043D30(void* arg0);
extern void func_80045FA4(void* arg0, s32 arg1);
extern void func_80046148(void* arg0, s32 arg1);
extern void func_80046188(void* arg0, void* arg1);
extern void func_80046218(u16 arg0, u16 arg1);
extern void func_800462F0(void);
extern s32 func_80046D5C(void);
extern s32 func_80046E6C(void);
extern void func_80047714(s32 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void func_8008105C(u16 actor_index, void* arg1, void* arg2);
extern void func_80081478(u16 actor_index, void* arg1, s32 arg2);
extern void func_80081790(u16 actor_index, void* arg1);
extern void func_800819A8(u16 actor_index, void* arg1);

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
