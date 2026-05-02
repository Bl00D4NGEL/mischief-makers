#include <PR/sp.h>

void spColor(Sprite* sp, u8 red, u8 green, u8 blue, u8 alpha) {
    sp->red = red;
    sp->green = green;
    sp->blue = blue;
    sp->alpha = alpha;
}
