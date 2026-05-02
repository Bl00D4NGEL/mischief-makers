#include <PR/libaudio.h>

void alCopy(void* src, void* dest, s32 len) {
    s32 index;
    u8* src_byte = (u8*)src;
    u8* dest_byte = (u8*)dest;

    for (index = 0; index < len; index++) {
        *dest_byte++ = *src_byte++;
    }
}
