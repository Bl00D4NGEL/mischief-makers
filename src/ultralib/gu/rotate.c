#include <PR/ultratypes.h>
#include <gu/guint.h>

extern float __cosf(float);
extern float __sinf(float);

void guRotateF(float mf[4][4], float angle, float x, float y, float z) {
    static float dtor = 3.1415926 / 180.0;
    float sine;
    float cosine;
    float ab;
    float bc;
    float ca;
    float t;

    guNormalize(&x, &y, &z);
    angle *= dtor;
    sine = __sinf(angle);
    cosine = __cosf(angle);
    t = 1.0f - cosine;
    ab = x * y * t;
    bc = y * z * t;
    ca = z * x * t;

    guMtxIdentF(mf);

    t = x * x;
    mf[0][0] = t + cosine * (1.0f - t);
    mf[2][1] = bc - x * sine;
    mf[1][2] = bc + x * sine;

    t = y * y;
    mf[1][1] = t + cosine * (1.0f - t);
    mf[2][0] = ca + y * sine;
    mf[0][2] = ca - y * sine;

    t = z * z;
    mf[2][2] = t + cosine * (1.0f - t);
    mf[1][0] = ab - z * sine;
    mf[0][1] = ab + z * sine;
}

void guRotate(Mtx* m, float angle, float x, float y, float z) {
    Matrix mf;

    guRotateF(mf, angle, x, y, z);
    guMtxF2L(mf, m);
}
