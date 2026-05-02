#include <PR/sp.h>

void spClearAttribute(Sprite* sp, s32 attr) {
    sp->attr &= ~attr;
}
