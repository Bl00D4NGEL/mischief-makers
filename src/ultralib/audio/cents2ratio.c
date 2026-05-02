#include <PR/libaudio.h>

f32 alCents2Ratio(s32 cents) {
    f32 pitch;
    f32 ratio = 1.0f;

    if (cents >= 0) {
        pitch = 1.00057779f;
    }
    else {
        pitch = 0.9994225441f;
        cents = -cents;
    }

    while (cents) {
        if (cents & 1) {
            ratio *= pitch;
        }

        pitch *= pitch;
        cents >>= 1;
    }

    return ratio;
}
