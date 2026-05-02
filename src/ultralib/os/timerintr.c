#include "PRinternal/macros.h"
#include "PR/os_internal.h"

extern OSThread* __osRunQueue;
extern void __osSetTimerIntr(OSTime time);
extern OSTime __osInsertTimer(OSTimer* timer);
extern void __osSetCompare(u32 value);
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 value);

OSTime __osCurrentTime;
u32 __osBaseCounter;
u32 __osViIntrCount;
u32 __osTimerCounter;
OSTimer __osBaseTimer;
OSTimer* __osTimerList = &__osBaseTimer;
OSProf* __osProfileList;
OSProf* __osProfileListEnd;
u32 __osProfileOverflowBin;
static OSMesgQueue __osProfTimerQ ALIGNED(8);

void __osTimerServicesInit(void) {
    __osCurrentTime = 0;
    __osBaseCounter = 0;
    __osViIntrCount = 0;
    __osTimerList->next = __osTimerList->prev = __osTimerList;
    __osTimerList->interval = __osTimerList->value = 0;
    __osTimerList->mq = NULL;
    __osTimerList->msg = 0;
}

void __osTimerInterrupt(void) {
    OSTimer* timer;
    u32 count;
    u32 elapsed_cycles;
    u32 pc;
    s32 offset;
    OSProf* prof;

    prof = __osProfileList;

    if (__osTimerList->next == __osTimerList) {
        return;
    }

    for (;;) {
        timer = __osTimerList->next;

        if (timer == __osTimerList) {
            __osSetCompare(0);
            __osTimerCounter = 0;
            break;
        }

        count = osGetCount();
        elapsed_cycles = count - __osTimerCounter;
        __osTimerCounter = count;

        if (elapsed_cycles < timer->value) {
            timer->value -= elapsed_cycles;
            __osSetTimerIntr(timer->value);
            break;
        }

        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        timer->next = NULL;
        timer->prev = NULL;

        if (timer->mq != NULL) {
            if (timer->mq != &__osProfTimerQ) {
                osSendMesg(timer->mq, timer->msg, OS_MESG_NOBLOCK);
            }
            else {
                pc = __osRunQueue->context.pc;
                for (prof = __osProfileList; prof < __osProfileListEnd; prof++) {
                    offset = pc - (u32)prof->text_start;

                    if (offset >= 0) {
                        if ((s32)prof->text_end - (s32)pc > 0) {
                            (*(u16*)(u32)((offset >> 2) + prof->histo_base))++;
                            goto ProfDone;
                        }
                    }
                }

                __osProfileOverflowBin++;
            }
        }

    ProfDone:
        if (timer->interval != 0) {
            timer->value = timer->interval;
            __osInsertTimer(timer);
        }
    }
}

void __osSetTimerIntr(OSTime time) {
    OSTime new_time;
    u32 saved_mask;

    saved_mask = __osDisableInt();
    __osTimerCounter = osGetCount();
    new_time = __osTimerCounter + time;
    __osSetCompare(new_time);
    __osRestoreInt(saved_mask);
}

OSTime __osInsertTimer(OSTimer* timer) {
    OSTimer* timep;
    OSTime time;
    u32 saved_mask;

    saved_mask = __osDisableInt();
    timep = __osTimerList->next;
    time = timer->value;

    for (; timep != __osTimerList && time > timep->value; timep = timep->next) {
        time -= timep->value;
    }

    timer->value = time;

    if (timep != __osTimerList) {
        timep->value -= time;
    }

    timer->next = timep;
    timer->prev = timep->prev;
    timep->prev->next = timer;
    timep->prev = timer;
    __osRestoreInt(saved_mask);
    return time;
}
