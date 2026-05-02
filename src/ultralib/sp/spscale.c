#include <PR/sp.h>

void spScale(Sprite* sp, f32 scale_x, f32 scale_y) {
    sp->scalex = scale_x;
    sp->scaley = scale_y;

    if ((scale_x == 1.0f) && (scale_y == 1.0f)) {
        spClearAttribute(sp, SP_SCALE);
    }
    else {
        spSetAttribute(sp, SP_SCALE);
    }
}
