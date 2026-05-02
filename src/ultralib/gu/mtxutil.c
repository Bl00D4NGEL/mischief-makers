#include <gu/guint.h>

void guMtxF2L(float mf[4][4], Mtx* m) {
    s32 row;
    s32 column;
    s32 upper;
    s32 lower;
    s32* integer_part;
    s32* fractional_part;

    integer_part = (s32*)&m->m[0][0];
    fractional_part = (s32*)&m->m[2][0];

    for (row = 0; row < 4; row++) {
        for (column = 0; column < 2; column++) {
            upper = FTOFIX32(mf[row][column * 2]);
            lower = FTOFIX32(mf[row][column * 2 + 1]);
            *integer_part++ = (upper & 0xFFFF0000) | ((lower >> 16) & 0xFFFF);
            *fractional_part++ = ((upper << 16) & 0xFFFF0000) | (lower & 0xFFFF);
        }
    }
}

void guMtxL2F(float mf[4][4], Mtx* m) {
    s32 row;
    s32 column;
    u32 upper;
    u32 lower;
    u32* integer_part;
    u32* fractional_part;
    s32 upper_fixed;
    s32 lower_fixed;

    integer_part = (u32*)&m->m[0][0];
    fractional_part = (u32*)&m->m[2][0];

    for (row = 0; row < 4; row++) {
        for (column = 0; column < 2; column++) {
            upper = (*integer_part & 0xFFFF0000) | ((*fractional_part >> 16) & 0xFFFF);
            lower = ((*integer_part++ << 16) & 0xFFFF0000) | (*fractional_part++ & 0xFFFF);
            upper_fixed = *(s32*)&upper;
            lower_fixed = *(s32*)&lower;

            mf[row][column * 2] = (float)upper_fixed / (float)0x10000;
            mf[row][column * 2 + 1] = (float)lower_fixed / (float)0x10000;
        }
    }
}

void guMtxIdentF(float mf[4][4]) {
    s32 row;
    s32 column;

    for (row = 0; row < 4; row++) {
        for (column = 0; column < 4; column++) {
            if (row == column) {
                mf[row][column] = 1.0f;
            }
            else {
                mf[row][column] = 0.0f;
            }
        }
    }
}

void guMtxIdent(Mtx* m) {
    Matrix mf;

    guMtxIdentF(mf);
    guMtxF2L(mf, m);
}
