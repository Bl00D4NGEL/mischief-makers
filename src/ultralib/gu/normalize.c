#include <gu/guint.h>

void guNormalize(float* x, float* y, float* z) {
    float magnitude;

    magnitude = 1.0f / sqrtf((*x * *x) + (*y * *y) + (*z * *z));
    *x *= magnitude;
    *y *= magnitude;
    *z *= magnitude;
}
