#include <gu/guint.h>

void guLookAtF(float mf[4][4], float x_eye, float y_eye, float z_eye, float x_at, float y_at, float z_at, float x_up, float y_up, float z_up) {
    float len;
    float x_look;
    float y_look;
    float z_look;
    float x_right;
    float y_right;
    float z_right;

    guMtxIdentF(mf);

    x_look = x_at - x_eye;
    y_look = y_at - y_eye;
    z_look = z_at - z_eye;

    // Negate because positive Z is behind us
    len = -1.0 / sqrtf(x_look * x_look + y_look * y_look + z_look * z_look);
    x_look *= len;
    y_look *= len;
    z_look *= len;

    x_right = y_up * z_look - z_up * y_look;
    y_right = z_up * x_look - x_up * z_look;
    z_right = x_up * y_look - y_up * x_look;
    len = 1.0 / sqrtf(x_right * x_right + y_right * y_right + z_right * z_right);
    x_right *= len;
    y_right *= len;
    z_right *= len;

    x_up = y_look * z_right - z_look * y_right;
    y_up = z_look * x_right - x_look * z_right;
    z_up = x_look * y_right - y_look * x_right;
    len = 1.0 / sqrtf(x_up * x_up + y_up * y_up + z_up * z_up);
    x_up *= len;
    y_up *= len;
    z_up *= len;

    mf[0][0] = x_right;
    mf[1][0] = y_right;
    mf[2][0] = z_right;
    mf[3][0] = -(x_eye * x_right + y_eye * y_right + z_eye * z_right);

    mf[0][1] = x_up;
    mf[1][1] = y_up;
    mf[2][1] = z_up;
    mf[3][1] = -(x_eye * x_up + y_eye * y_up + z_eye * z_up);

    mf[0][2] = x_look;
    mf[1][2] = y_look;
    mf[2][2] = z_look;
    mf[3][2] = -(x_eye * x_look + y_eye * y_look + z_eye * z_look);

    mf[0][3] = 0;
    mf[1][3] = 0;
    mf[2][3] = 0;
    mf[3][3] = 1;
}

void guLookAt(Mtx* m, float x_eye, float y_eye, float z_eye, float x_at, float y_at, float z_at, float x_up, float y_up, float z_up) {
    Matrix mf;

    guLookAtF(mf, x_eye, y_eye, z_eye, x_at, y_at, z_at, x_up, y_up, z_up);
    guMtxF2L(mf, m);
}
