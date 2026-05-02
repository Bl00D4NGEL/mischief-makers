#include "PR/os_internal.h"
#include "PR/ultraerror.h"
#include "PR/sptask.h"
#include "PR/rcp.h"
#include "PRinternal/osint.h"

#define _osVirtualToPhysical(ptr)              \
    if (ptr != NULL) {                         \
        ptr = (void*)osVirtualToPhysical(ptr); \
    }                                          \
    (void)0

static OSTask sTmpTask;

static OSTask* _VirtualToPhysicalTask(OSTask* task) {
    OSTask* physical_task;

    physical_task = &sTmpTask;
    bcopy(task, physical_task, sizeof(OSTask));

    _osVirtualToPhysical(physical_task->t.ucode);
    _osVirtualToPhysical(physical_task->t.ucode_data);
    _osVirtualToPhysical(physical_task->t.dram_stack);
    _osVirtualToPhysical(physical_task->t.output_buff);
    _osVirtualToPhysical(physical_task->t.output_buff_size);
    _osVirtualToPhysical(physical_task->t.data_ptr);
    _osVirtualToPhysical(physical_task->t.yield_data_ptr);

    return physical_task;
}

void osSpTaskLoad(OSTask* task) {
    OSTask* physical_task;

    if ((task->t.dram_stack != NULL) && ((u32)task->t.dram_stack & 0xF)) {
        __osError(ERR_OSSPTASKLOAD_DRAM, 1, task->t.dram_stack);
        return;
    }

    if ((task->t.output_buff != NULL) && ((u32)task->t.output_buff & 0xF)) {
        __osError(ERR_OSSPTASKLOAD_OUT, 1, task->t.output_buff);
        return;
    }

    if ((task->t.output_buff_size != NULL) && ((u32)task->t.output_buff_size & 0xF)) {
        __osError(ERR_OSSPTASKLOAD_OUTSIZE, 1, task->t.output_buff_size);
        return;
    }

    if ((task->t.yield_data_ptr != NULL) && ((u32)task->t.yield_data_ptr & 0xF)) {
        __osError(ERR_OSSPTASKLOAD_YIELD, 1, task->t.yield_data_ptr);
        return;
    }

    physical_task = _VirtualToPhysicalTask(task);

    if (physical_task->t.flags & OS_TASK_YIELDED) {
        physical_task->t.ucode_data = physical_task->t.yield_data_ptr;
        physical_task->t.ucode_data_size = physical_task->t.yield_data_size;
        task->t.flags &= ~OS_TASK_YIELDED;

        if (physical_task->t.flags & OS_TASK_LOADABLE) {
            physical_task->t.ucode = (u64*)IO_READ((u32)task->t.yield_data_ptr + OS_YIELD_DATA_SIZE - 4);
        }
    }

    osWritebackDCache(physical_task, sizeof(OSTask));
    __osSpSetStatus(SP_CLR_YIELD | SP_CLR_YIELDED | SP_CLR_TASKDONE | SP_SET_INTR_BREAK);

    while (__osSpSetPc(SP_IMEM_START) == -1) {
    }

    while (__osSpRawStartDma(OS_WRITE, SP_IMEM_START - sizeof(*physical_task), physical_task, sizeof(OSTask)) == -1) {
    }

    while (__osSpDeviceBusy()) {
    }

    while (__osSpRawStartDma(OS_WRITE, SP_IMEM_START, physical_task->t.ucode_boot,
                             physical_task->t.ucode_boot_size) == -1) {
    }
}

void osSpTaskStartGo(OSTask* task) {
    while (__osSpDeviceBusy()) {
    }

    __osSpSetStatus(SP_SET_INTR_BREAK | SP_CLR_SSTEP | SP_CLR_BROKE | SP_CLR_HALT);
}
