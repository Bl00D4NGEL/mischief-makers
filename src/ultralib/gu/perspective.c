#include <PR/ultratypes.h>
#include <gu/guint.h>

extern float __cosf(float);
extern float __sinf(float);

void guPerspectiveF(float mf[4][4], u16* persp_norm, float fovy, float aspect, float near, float far, float scale) {
    float cot;
    int row;
    int col;

    guMtxIdentF(mf);

    fovy *= 3.1415926 / 180.0;
    cot = __cosf(fovy / 2) / __sinf(fovy / 2);

    mf[0][0] = cot / aspect;
    mf[1][1] = cot;
    mf[2][2] = (near + far) / (near - far);
    mf[2][3] = -1;
    mf[3][2] = (2 * near * far) / (near - far);
    mf[3][3] = 0;

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            mf[row][col] *= scale;
        }
    }

    if (persp_norm != (u16*)NULL) {
        if (near + far <= 2.0) {
            *persp_norm = (u16)0xFFFF;
        }
        else {
            *persp_norm = (u16)((2.0 * 65536.0) / (near + far));
            if (*persp_norm <= 0) {
                *persp_norm = (u16)0x0001;
            }
        }
    }
}

void guPerspective(Mtx* m, u16* persp_norm, float fovy, float aspect, float near, float far, float scale) {
    Matrix mf;

    guPerspectiveF(mf, persp_norm, fovy, aspect, near, far, scale);
    guMtxF2L(mf, m);
}
