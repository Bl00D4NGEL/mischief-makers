#include <PR/sp.h>

void spSetAttribute(Sprite* sp, s32 attr) {
    sp->attr |= attr;
}
