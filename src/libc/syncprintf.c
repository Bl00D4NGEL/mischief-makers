#include <compiler/ido/stdarg.h>
#include <PR/os.h>
#include <PR/rcp.h>
#include <PR/rdb.h>

int _Printf(void* pfn(void*, const char*, u32), void* arg, const char* fmt, va_list ap);

extern u32 __kmc_pt_mode;

static void* proutSyncPrintf(void* str, const char* buf, u32 count) {
    u32 sent = 0;

    while (sent < count) {
        sent += __osRdbSend(buf + sent, count - sent, RDB_TYPE_GtoH_PRINT);
    }
    return (void*)1;
}

static volatile unsigned int* stat = (unsigned*)0xBFF08004;
static volatile unsigned int* wport = (unsigned*)0xBFF08000;
static volatile unsigned int* piok = (unsigned*)PHYS_TO_K1(PI_STATUS_REG);

static void rmonPutchar(char c) {
    while (*piok & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY)) {
    }

    while (!(*stat & 4)) {
    }

    *wport = c;
}

static void* kmc_proutSyncPrintf(void* str, const char* buf, int count) {
    int index;
    char c;
    char* p;
    char* q;
    char xbuf[128];
    static int column = 0;

    p = xbuf;

    for (index = 0; index < count; index++) {
        c = *buf++;

        switch (c) {
            case '\n':
                *p++ = '\n';
                column = 0;
                break;

            case '\t':
                do {
                    *p++ = ' ';
                } while (++column % 8);
                break;

            default:
                column++;
                *p++ = c;
                break;
        }

        if (c == '\n' || (p - xbuf) > 100) {
            rmonPutchar((p - xbuf) - 1);

            q = xbuf;
            while (q != p) {
                rmonPutchar(*q++);
            }
            p = xbuf;
        }
    }

    if (p != xbuf) {
        rmonPutchar((p - xbuf) - 1);

        q = xbuf;
        while (q != p) {
            rmonPutchar(*q++);
        }
    }

    return (void*)1;
}

void osSyncPrintf(const char* fmt, ...) {
    int ans;
    va_list ap;

    va_start(ap, fmt);
    if (__kmc_pt_mode) {
        ans = _Printf(kmc_proutSyncPrintf, NULL, fmt, ap);
    }
    else {
        ans = _Printf(proutSyncPrintf, NULL, fmt, ap);
    }
    va_end(ap);
}

void rmonPrintf(const char* fmt, ...) {
    int ans;
    va_list ap;

    va_start(ap, fmt);
    if (__kmc_pt_mode) {
        ans = _Printf(kmc_proutSyncPrintf, NULL, fmt, ap);
    }
    va_end(ap);
}
