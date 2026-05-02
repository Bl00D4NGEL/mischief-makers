#include "common.h"
#include "actor.h"

extern s16 D_800D5830;
extern s16 D_800D5834;
extern u8 D_800E1180[];
extern u8 D_800E45D0[];
extern u8 D_800E4698[];
extern u8 D_800E574C[];
extern u8 D_800E57D4[];
extern u8 D_800E58D4[];
extern u8 D_800E58F0[];
extern u8 D_800E5910[];
extern u8 D_800E5938[];
extern u8 D_800E5970[];
extern u8 D_800E59E0[];

extern s32 func_800036C8(s32 arg0, u16 arg1);
extern s32 func_80003980(s32 arg0, u16 arg1);
extern s32 func_8001E2D0(u16 arg0);
extern u16 func_80028528(void);
extern s32 func_8002981C(s32 arg0, s32 arg1, s32 arg2);
extern void func_8002AEB4(u16 arg0, s32 arg1);
extern s16 func_8005DEFC(void);
extern void func_8005DF40(s32 arg0, s32 arg1);
extern s32 func_8005DF5C(s32 arg0);
extern void func_8007F9E0(u16 actor_index);
extern void func_8008105C(u16 arg0, void* arg1, void* arg2);
extern void func_80081478(u16 arg0, void* arg1, s32 arg2);
extern void func_80081720(u16 arg0, void* arg1, s32 arg2);
extern void func_80081790(u16 arg0, void* arg1);
extern void func_800819A8(u16 arg0, void* arg1);
extern void func_8008C710(u16 arg0);

void func_80192100_6D1C00(u16 actor_index);
void func_801921D4_6D1CD4(u16 actor_index);
void func_80192418_6D1F18(u16 actor_index);
void func_80192460_6D1F60(u16 actor_index, u16 other_actor_index);
void func_801924F4_6D1FF4(u16 actor_index);
void func_80192C00_6D2700(u16 actor_index);

void func_80192100_6D1C00(u16 actor_index) {
    D_800EF510[actor_index].unk_094 |= 0x11;
    D_800EF510[actor_index].unk_080 = 3;
    func_8008105C(actor_index, D_800E4698, D_800E45D0);
    D_800EF510[actor_index].unk_178 = D_800E574C;
    func_80081478(actor_index, D_800E4698, 0);
    func_80081790(actor_index, D_800E57D4);
    func_800819A8(actor_index, D_800E4698);
    D_800EF510[actor_index].unk_09F = 0xFE;
    func_8002AEB4(actor_index, 4);
}

void func_801921D4_6D1CD4(u16 actor_index) {
    if (D_800EF510[actor_index].unk_0D0 != 0) {
        if (D_800EF510[actor_index + 1].unk_180 & 0x8000) {
            func_800819A8(actor_index, D_800E4698);
            func_8008C710(actor_index);
            func_8002AEB4(actor_index + 0xB, 0x18);
            func_8002AEB4(actor_index + 0xF, 0x18);
            func_8002AEB4(actor_index + 0x11, 0x30);
            func_8002AEB4(actor_index + 0x12, 0x18);
            func_8002AEB4(actor_index + 9, 0x18);
            func_8002AEB4(actor_index + 0x1C, 0x18);
            func_8002AEB4(actor_index + 0x1E, 0x18);
            D_800EF510[actor_index + 18].unk_0B8 = (f32)((f64)D_800EF510[actor_index + 18].unk_0B8 * 1.25);
            D_800EF510[actor_index + 10].unk_0B8 = (f32)((f64)D_800EF510[actor_index + 10].unk_0B8 * 1.25);
            D_800EF510[actor_index + 28].unk_0B4 = (f32)((f64)D_800EF510[actor_index + 28].unk_0B4 * 0.95);
            D_800EF510[actor_index + 28].unk_0B8 = (f32)((f64)D_800EF510[actor_index + 28].unk_0B8 * 0.95);
            D_800EF510[actor_index + 30].unk_0B4 = (f32)((f64)D_800EF510[actor_index + 30].unk_0B4 * 0.95);
            D_800EF510[actor_index + 30].unk_0B8 = (f32)((f64)D_800EF510[actor_index + 30].unk_0B8 * 0.95);
            D_800EF510[actor_index + 15].unk_0B4 = (f32)((f64)D_800EF510[actor_index + 15].unk_0B4 * 0.95);
            D_800EF510[actor_index + 15].unk_0B8 = (f32)((f64)D_800EF510[actor_index + 15].unk_0B8 * 0.95);
            D_800EF510[actor_index + 17].unk_0B4 = (f32)((f64)D_800EF510[actor_index + 17].unk_0B4 * 0.95);
            D_800EF510[actor_index + 17].unk_0B8 = (f32)((f64)D_800EF510[actor_index + 17].unk_0B8 * 0.95);
            D_800EF510[actor_index + 18].unk_0B4 = (f32)((f64)D_800EF510[actor_index + 18].unk_0B4 * 0.95);
            D_800EF510[actor_index + 18].unk_0B8 = (f32)((f64)D_800EF510[actor_index + 18].unk_0B8 * 0.95);
        }
    }
}

void func_80192418_6D1F18(u16 actor_index) {
    D_800EF510[actor_index].unk_098 &= 0xFFDFF9FF;
}

void func_80192460_6D1F60(u16 actor_index, u16 other_actor_index) {
    D_800EF510[other_actor_index].unk_088 = D_800EF510[actor_index + 1].unk_088;
    D_800EF510[other_actor_index].unk_08C = D_800EF510[actor_index + 1].unk_08C + 0x24;
    *(s32*)&D_800EF510[other_actor_index].unk_090 = *(s32*)&D_800EF510[actor_index + 1].unk_090;
    func_8007F9E0(other_actor_index);
}

// Thanks to LLONSIT for helping with this one!
void func_801924F4_6D1FF4(u16 actor_index) {
    u16 temp;
    u16 temp_index_22;
    u16 temp_index_21;

    temp_index_21 = actor_index + 0x21;
    temp_index_22 = actor_index + 0x22;
    func_801921D4_6D1CD4(actor_index);

    switch (D_800EF510[actor_index].unk_0D0) {
        case 0:
            D_800EF510[actor_index + 1].unk_180 = 0;
            if (((u16)D_800EF510[actor_index].unk_110) == 0) {
                D_800EF510[actor_index].unk_0D0 = 0x4000;
            }
            break;

        case 0x4000:
            D_800EF510[actor_index].unk_0D0++;
            D_800EF510[actor_index].unk_088 = -0x100;
            D_800EF510[actor_index].unk_08C = -0x30;
            D_800EF510[actor_index].unk_090 = 0;
            D_800EF510[actor_index + 2].unk_180 = 0x9E;

        case 0x4001:
            D_800EF510[actor_index + 2].unk_180--;
            if (D_800EF510[actor_index + 2].unk_180 < 0) {
                func_80192100_6D1C00(actor_index);
                D_800EF510[actor_index].unk_0D0++;
                D_800EF510[actor_index + 1].unk_180 |= 0x8000;
                D_800EF510[actor_index].unk_0EC = 0x48000;
                D_800EF510[actor_index].unk_0F0 = 0xC000;
                func_80081790(actor_index, D_800E58D4);
                D_800EF510[temp_index_22].unk_0D2 = 0x1D;
                func_8001E2D0(temp_index_22);
                D_800EF510[temp_index_22].unk_150 = D_800E1180;
                D_800EF510[temp_index_22].unk_154 = 1;
                D_800EF510[temp_index_22].unk_080 = 3;
            }
            break;

        case 0x4002:
            if (D_800EF510[actor_index].unk_088 >= -0x3F) {
                D_800EF510[actor_index].unk_0D0++;
                func_80081790(actor_index, D_800E58F0);
            }
            func_80192460_6D1F60(actor_index, temp_index_22);
            break;

        case 0x4003:
            D_800EF510[actor_index].unk_0EC -= 0x2000;
            if (D_800EF510[actor_index].unk_0EC < -0x1C000) {
                D_800EF510[actor_index].unk_0D0++;
            }
            func_80192460_6D1F60(actor_index, temp_index_22);
            break;

        case 0x4004:
            D_800EF510[actor_index].unk_0EC += 0x1800;
            D_800EF510[actor_index].unk_0F0 -= 0x1000;
            if (D_800EF510[actor_index].unk_0F0 < -0x10000) {
                D_800EF510[actor_index].unk_0D0++;
                D_800EF510[actor_index + 2].unk_180 = 0x14;
                D_800EF510[actor_index + 3].unk_180 = 0;
                func_80081790(actor_index, D_800E5910);
            }
            func_80192460_6D1F60(actor_index, temp_index_22);
            break;

        case 0x4005:
            D_800EF510[actor_index + 2].unk_180--;
            if (D_800EF510[actor_index + 2].unk_180 < 0) {
                if (func_8005DEFC() == 0) {
                    switch (D_800EF510[actor_index + 3].unk_180) {
                        case 0:
                            func_800036C8(0x44, actor_index);
                            D_800EF510[temp_index_21].unk_0E8 = D_800E59E0;
                            D_800EF510[temp_index_21].unk_0E6 = 1;
                            func_8005DF5C(1);
                            D_800D5830 = -0x68;
                            D_800D5834 = 0x26;
                            func_8005DF40(0, -0x30);
                            break;

                        case 1:
                            func_8005DF5C(2);
                            D_800D5830 = 0x40;
                            D_800EF510[actor_index + 2].unk_180 = 0x1E;
                            break;

                        case 2:
                            func_8005DF5C(3);
                            D_800D5830 = -0x68;
                            D_800D5834 = 0x26;
                            D_800EF510[actor_index + 2].unk_180 = 0x1E;
                            break;

                        case 3:
                            func_8005DF5C(4);
                            D_800D5830 = 0x40;
                            D_800EF510[actor_index + 2].unk_180 = 0x3C;
                            break;

                        case 4:
                            D_800EF510[actor_index].unk_0D0++;
                            D_800EF510[temp_index_21].unk_0E8 = D_800E5970;
                            D_800EF510[temp_index_21].unk_0E6 = 1;
                            D_800EF510[actor_index].unk_0EC = -0x1A000;
                            D_800EF510[actor_index].unk_0F0 = -0x18000;
                            func_80081790(actor_index, D_800E5938);
                            temp = func_80028528();
                            if (temp != 0) {
                                D_800EF510[temp].unk_0D2 = 0x34;
                                func_8001E2D0(temp);
                                D_800EF510[temp].unk_088 = 0;
                                D_800EF510[temp].unk_0EC = 0x10000;
                                D_800EF510[temp].unk_148 = 300.0f;
                                func_80003980(0xDD, temp);
                                func_800036C8(0xB0, actor_index);
                            }
                    }
                    D_800EF510[actor_index + 3].unk_180++;
                }
            }
            D_800EF510[actor_index].unk_0EC = func_8002981C(D_800EF510[actor_index].unk_0EC, 0, 0x1000);
            D_800EF510[actor_index].unk_0F0 = func_8002981C(D_800EF510[actor_index].unk_0F0, 0, 0x1000);
            func_80192460_6D1F60(actor_index, temp_index_22);
            break;

        case 0x4006:
            if (D_800EF510[actor_index].unk_088 >= 0x241) {
                D_800EF510[actor_index].unk_080 = 0;
                func_80081720(actor_index, D_800E4698, 0);
                D_800EF510[actor_index + 0x22].unk_080 = 0;
                return;
            }
            D_800EF510[actor_index].unk_0EC += 0x2800;
            D_800EF510[actor_index].unk_0F0 += 0x1200;
            func_80192460_6D1F60(actor_index, temp_index_22);
            break;
    }

    func_80192418_6D1F18(actor_index);
}

void func_80192C00_6D2700(u16 actor_index) {
}
