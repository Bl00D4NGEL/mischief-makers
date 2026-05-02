#include <PR/ultratypes.h>
#include <compiler/ido/string.h>

char* strchr(const char* str, int ch) {
    unsigned char c;

    c = ch;
    if (*str != c) {
        do {
            if (*str == 0) {
                return NULL;
            }
        } while (str++, *str != c);
    }
    return (char*)str;
}

size_t strlen(const char* str) {
    const char* cur;

    cur = str;
    if (*cur != 0) {
        do {
        } while (*++cur != 0);
    }
    return cur - str;
}

void* memcpy(void* dest, const void* src, size_t count) {
    char* dest_cur;
    const char* src_cur;

    dest_cur = dest;
    if (src_cur = src, count != 0) {
        do {
            *dest_cur = *src_cur;
        } while (dest_cur++, src_cur++, --count != 0);
    }
    return dest;
}
