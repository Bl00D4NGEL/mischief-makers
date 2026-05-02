#include <audio/synthInternals.h>
#include <PR/libaudio.h>
#include <PR/os_internal.h>
#include <PR/ultraerror.h>

void* alHeapDBAlloc(u8* file, s32 line, ALHeap* heap, s32 num, s32 size) {
    s32 bytes;
    u8* ptr = 0;

    bytes = ((num * size) + AL_CACHE_ALIGN) & ~AL_CACHE_ALIGN;
    heap->count++;
    bytes += sizeof(HeapInfo);

    if ((heap->cur + bytes) <= (heap->base + heap->len)) {
        ptr = heap->cur;
        heap->cur += bytes;

        ((HeapInfo*)ptr)->magic = AL_HEAP_MAGIC;
        ((HeapInfo*)ptr)->size = bytes;
        ((HeapInfo*)ptr)->count = heap->count;

        if (file) {
            ((HeapInfo*)ptr)->file = file;
            ((HeapInfo*)ptr)->line = line;
        }
        else {
            ((HeapInfo*)ptr)->file = (u8*)"unknown";
            ((HeapInfo*)ptr)->line = 0;
        }

        ptr += sizeof(HeapInfo);
    }
    else {
        __osError(ERR_ALHEAPNOFREE, 1, size);
    }

    return ptr;
}
