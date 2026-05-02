#include <gu/guint.h>

void guOrthoF(float mf[4][4], float left, float right, float bottom, float top, float near, float far, float scale) {
    int row;
    int col;

    guMtxIdentF(mf);

    mf[0][0] = 2 / (right - left);
    mf[1][1] = 2 / (top - bottom);
    mf[2][2] = -2 / (far - near);
    mf[3][0] = -(right + left) / (right - left);
    mf[3][1] = -(top + bottom) / (top - bottom);
    mf[3][2] = -(far + near) / (far - near);
    mf[3][3] = 1;

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            mf[row][col] *= scale;
        }
    }
}

void guOrtho(Mtx* m, float left, float right, float bottom, float top, float near, float far, float scale) {
    Matrix mf;

    guOrthoF(mf, left, right, bottom, top, near, far, scale);
    guMtxF2L(mf, m);
}
