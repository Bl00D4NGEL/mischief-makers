#include <compiler/ido/memory.h>
#include <compiler/ido/stdarg.h>

int _Printf(void* pfn(void*, const char*, size_t), void* arg, const char* fmt, va_list ap);

static void* proutSprintf(void* str, const char* buf, size_t count);

int sprintf(char* str, const char* fmt, ...) {
    int ans;
    va_list ap;

    va_start(ap, fmt);
    ans = _Printf(proutSprintf, str, fmt, ap);
    if (ans >= 0) {
        str[ans] = 0;
    }
    return ans;
}

static void* proutSprintf(void* str, const char* buf, size_t count) {
    return (char*)memcpy(str, buf, count) + count;
}
