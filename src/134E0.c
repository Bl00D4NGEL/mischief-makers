#include "common.h"

typedef u8 (*UnkFuncType_D_800C7CD0)(s16 arg0, s16 arg1);

u8 func_800128E0(s16 arg0, s16 arg1);
u8 func_800128F0(s16 arg0, s16 arg1);
u8 func_80012944(s16 arg0, s16 arg1);
u8 func_80012970(s16 arg0, s16 arg1);
u8 func_800129C8(s16 arg0, s16 arg1);
u8 func_80012A24(s16 arg0, s16 arg1);
u8 func_80012A58(s16 arg0, s16 arg1);
u8 func_80012AB4(s16 arg0, s16 arg1);

UnkFuncType_D_800C7CD0 D_800C7CD0[] = {
    func_800128E0,
    func_800128F0,
    func_80012944,
    func_800128E0,
    func_800128E0,
    func_80012970,
    func_800128E0,
    func_800128E0,
    func_800128E0,
    func_800129C8,
    func_80012A24,
    func_800128E0,
    func_800128E0,
    func_80012A58,
    func_800128E0,
    func_800128E0,
};

u8 func_800128E0(s16 arg0, s16 arg1) {
    return 0xFF;
}

#pragma GLOBAL_ASM("asm/nonmatchings/134E0/func_800128F0.s")

u8 func_80012944(s16 arg0, s16 arg1) {
    if ((arg0 & 0xF) > (arg1 & 0xF)) {
        return 0xFF;
    }

    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/134E0/func_80012970.s")

#pragma GLOBAL_ASM("asm/nonmatchings/134E0/func_800129C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/134E0/func_80012A24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/134E0/func_80012A58.s")

#pragma GLOBAL_ASM("asm/nonmatchings/134E0/func_80012AB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/134E0/func_80012B28.s")

u8 func_80012B88(u8 arg0, s16 arg1, s16 arg2) {
    return D_800C7CD0[arg0 & 0xF](D_800BE558 + arg1, D_800BE55C + arg2) & arg0;
}

u8 func_80012C04(s16 arg0, s16 arg1) {
    return func_80012B88(func_80012AB4(arg0, arg1), arg0, arg1);
}
