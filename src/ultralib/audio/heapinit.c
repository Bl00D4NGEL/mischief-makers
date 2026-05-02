#include <audio/synthInternals.h>
#include <PR/libaudio.h>

void alHeapInit(ALHeap* heap, u8* base, s32 len) {
    s32 extra_align = (AL_CACHE_ALIGN + 1) - ((s32)base & AL_CACHE_ALIGN);

    if (extra_align != (AL_CACHE_ALIGN + 1)) {
        heap->base = base + extra_align;
    }
    else {
        heap->base = base;
    }

    heap->len = len;
    heap->cur = heap->base;
    heap->count = 0;
}
