#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PR/os_version.h"

typedef struct {
    /* 0x0 */ unsigned int inst1;
    /* 0x4 */ unsigned int inst2;
    /* 0x8 */ unsigned int inst3;
    /* 0xC */ unsigned int inst4;
} __osExceptionVector;

extern __osExceptionVector __osExceptionPreamble[];
extern __osExceptionVector __ptExceptionPreamble[];

// data
OSTime osClockRate = OS_CLOCK_RATE;
s32 osViClock = VI_NTSC_CLOCK;
u32 __osShutdown = 0;
u32 __OSGlobalIntMask = OS_IM_ALL;

// bss
u32 __kmc_pt_mode;

void __osInitialize_common(void) {
    u32 pifdata;
    u32 clock = 0;

    __osSetSR(__osGetSR() | SR_CU1);    // enable fpu
    __osSetFpcCsr(FPCSR_FS | FPCSR_EV); // flush denorm to zero, enable invalid operation

    while (__osSiRawReadIo(PIF_RAM_END - 3, &pifdata)) { // last byte of joychannel ram
        ;
    }
    while (__osSiRawWriteIo(PIF_RAM_END - 3, pifdata | 8)) {
        ; // todo: magic contant
    }
    *(__osExceptionVector*)UT_VEC = *__osExceptionPreamble;
    *(__osExceptionVector*)XUT_VEC = *__osExceptionPreamble;
    *(__osExceptionVector*)ECC_VEC = *__osExceptionPreamble;
    *(__osExceptionVector*)E_VEC = *__osExceptionPreamble;
    osWritebackDCache((void*)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    osInvalICache((void*)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));

    osMapTLBRdb();
    osPiRawReadIo(4, &clock); // Read clock rate from the ROM header
    clock &= ~0xf;
    if (clock != 0) {
        osClockRate = clock;
    }
    osClockRate = osClockRate * 3 / 4;

    if (osResetType == 0) { // cold reset
        bzero(osAppNMIBuffer, OS_APP_NMI_BUFSIZE);
    }

    if (osTvType == OS_TV_PAL) {
        osViClock = VI_PAL_CLOCK;
    }
    else if (osTvType == OS_TV_MPAL) {
        osViClock = VI_MPAL_CLOCK;
    }
    else {
        osViClock = VI_NTSC_CLOCK;
    }

    if (!__kmc_pt_mode) {
        int (*fnc)();
        unsigned int c;
        unsigned int c1;
        unsigned int* src;
        unsigned int* dst;
        unsigned int monadr;
        volatile unsigned int* mon;
        volatile unsigned int* stat;

        stat = (unsigned*)0xbff08004;
        mon = (unsigned*)0xBFF00000;
        if (*mon != 0x4B4D4300) {
            return;
        }

        src = (unsigned*)__ptExceptionPreamble;
        dst = (unsigned*)E_VEC;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        src += 2;
        dst += 2;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;

        osWritebackDCache(E_VEC, 0x24);
        osInvalICache(E_VEC, 0x24);

        __kmc_pt_mode = TRUE;

        if ((*stat & 0x10) == 0) {
            monadr = *(mon + 1);
            if (monadr != 0xBFF00000) {
                unsigned int* src;
                unsigned int* dst = monadr | 0x20000000;
                unsigned int ct = 0x2000 / 4;

                src = 0xBFF00000;

                while (ct != 0) {
                    *dst++ = *src++;
                    ct--;
                }
            }
            fnc = monadr + 8;
            fnc(0x4B4D4300, 0);
        }
    }
}

void ptstart() {
}

