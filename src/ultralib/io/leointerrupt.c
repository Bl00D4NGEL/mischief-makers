#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/macros.h"
#include "PRinternal/osint.h"
#include "piint.h"

extern OSPiHandle* __osDiskHandle;

u8 leoDiskStack[OS_PIM_STACKSIZE] ALIGNED(0x10);

static void __osLeoAbnormalResume(void);
static void __osLeoResume(void);

s32 __osLeoInterrupt(void) {
    u32 stat;
    volatile u32 pi_stat;
    u32 bm_stat;
    __OSTranxInfo* info;
    __OSBlockInfo* block_info;

    stat = 0;
    info = &__osDiskHandle->transferInfo;
    block_info = &info->block[info->blockNum];

    pi_stat = IO_READ(PI_STATUS_REG);
    if (pi_stat & PI_STATUS_DMA_BUSY) {
        __OSGlobalIntMask &= ~SR_IBIT4;
        block_info->errStatus = LEO_ERROR_29;
        __osLeoResume();
        return 1;
    }

    WAIT_ON_IOBUSY(pi_stat);
    stat = IO_READ(LEO_STATUS);
    if (stat & LEO_STATUS_MECHANIC_INTERRUPT) {
        WAIT_ON_IOBUSY(pi_stat);
        IO_WRITE(LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_CLR_MECHANIC_INTR);
        block_info->errStatus = LEO_ERROR_GOOD;
        return 0;
    }

    if (info->cmdType == LEO_CMD_TYPE_2) {
        return 1;
    }

    if (stat & LEO_STATUS_BUFFER_MANAGER_ERROR) {
        WAIT_ON_IOBUSY(pi_stat);
        stat = IO_READ(LEO_STATUS);
        block_info->errStatus = LEO_ERROR_22;
        __osLeoResume();
        IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
        __OSGlobalIntMask |= OS_IM_PI;
        return 1;
    }

    if (info->cmdType == LEO_CMD_TYPE_1) {
        if ((stat & LEO_STATUS_DATA_REQUEST) == 0) {
            if (info->sectorNum + 1 != info->transferMode * 85) {
                block_info->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
                return 1;
            }

            IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
            __OSGlobalIntMask |= OS_IM_PI;
            block_info->errStatus = LEO_ERROR_GOOD;
            __osLeoResume();
            return 1;
        }
        else {
            block_info->dramAddr = (void*)((u32)block_info->dramAddr + block_info->sectorSize);
            info->sectorNum++;
            __osEPiRawStartDma(__osDiskHandle, OS_WRITE, LEO_SECTOR_BUFF, block_info->dramAddr,
                               block_info->sectorSize);
            return 1;
        }
    }
    else if (info->cmdType == LEO_CMD_TYPE_0) {
        if (info->transferMode == LEO_SECTOR_MODE) {
            if (info->sectorNum > (s32)block_info->C1ErrNum + 17) {
                block_info->errStatus = LEO_ERROR_GOOD;
                __osLeoAbnormalResume();
                return 1;
            }

            if ((stat & LEO_STATUS_DATA_REQUEST) == 0) {
                block_info->errStatus = LEO_ERROR_23;
                __osLeoAbnormalResume();
                return 1;
            }
        }
        else {
            block_info->dramAddr = (void*)((u32)block_info->dramAddr + block_info->sectorSize);
        }

        bm_stat = IO_READ(LEO_BM_STATUS);
        if (((bm_stat & LEO_BM_STATUS_C1SINGLE) && (bm_stat & LEO_BM_STATUS_C1DOUBLE)) ||
            (bm_stat & LEO_BM_STATUS_MICRO)) {
            if (block_info->C1ErrNum > 3) {
                if ((info->transferMode != LEO_SECTOR_MODE) || (info->sectorNum > 0x52)) {
                    block_info->errStatus = LEO_ERROR_23;
                    __osLeoAbnormalResume();
                    return 1;
                }
            }
            else {
                s32 err_num;

                err_num = block_info->C1ErrNum;
                block_info->C1ErrSector[err_num] = info->sectorNum + 1;
            }

            block_info->C1ErrNum++;
        }

        if (stat & LEO_STATUS_C2_TRANSFER) {
            if (info->sectorNum + 1 != 88) {
                block_info->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
            }

            if ((info->transferMode == LEO_TRACK_MODE) && (info->blockNum == 0)) {
                info->blockNum = 1;
                info->sectorNum = -1;
                info->block[1].dramAddr = (void*)((u32)info->block[1].dramAddr - info->block[1].sectorSize);
                block_info->errStatus = LEO_ERROR_22;
            }
            else {
                IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
                __OSGlobalIntMask |= OS_IM_PI;
                info->cmdType = LEO_CMD_TYPE_2;
                block_info->errStatus = LEO_ERROR_GOOD;
            }

            __osEPiRawStartDma(__osDiskHandle, OS_READ, LEO_C2_BUFF, block_info->C2Addr,
                               block_info->sectorSize * 4);
            return 1;
        }

        if ((info->sectorNum == -1) && (info->transferMode == LEO_TRACK_MODE) && (info->blockNum == 1)) {
            __OSBlockInfo* first_block_info;

            first_block_info = &info->block[0];
            if (first_block_info->C1ErrNum == 0) {
                if (((u32*)first_block_info->C2Addr)[0] | ((u32*)first_block_info->C2Addr)[1] |
                    ((u32*)first_block_info->C2Addr)[2] | ((u32*)first_block_info->C2Addr)[3]) {
                    first_block_info->errStatus = LEO_ERROR_24;
                    __osLeoAbnormalResume();
                    return 1;
                }
            }

            first_block_info->errStatus = LEO_ERROR_GOOD;
            __osLeoResume();
        }

        info->sectorNum++;
        if (stat & LEO_STATUS_DATA_REQUEST) {
            if (info->sectorNum > 0x54) {
                block_info->errStatus = LEO_ERROR_24;
                __osLeoAbnormalResume();
                return 1;
            }

            __osEPiRawStartDma(__osDiskHandle, OS_READ, LEO_SECTOR_BUFF, block_info->dramAddr,
                               block_info->sectorSize);
            block_info->errStatus = LEO_ERROR_GOOD;
            return 1;
        }
        else if (info->sectorNum <= 0x54) {
            block_info->errStatus = LEO_ERROR_24;
            __osLeoAbnormalResume();
            return 1;
        }

        return 1;
    }
    else {
        block_info->errStatus = LEO_ERROR_4;
        __osLeoAbnormalResume();
        return 1;
    }
}

static void __osLeoAbnormalResume(void) {
    __OSTranxInfo* info;
    u32 pi_stat;

    info = &__osDiskHandle->transferInfo;

    WAIT_ON_IOBUSY(pi_stat);
    IO_WRITE(LEO_BM_CTL, info->bmCtlShadow | LEO_BM_CTL_RESET);
    WAIT_ON_IOBUSY(pi_stat);
    IO_WRITE(LEO_BM_CTL, info->bmCtlShadow);
    __osLeoResume();
    IO_WRITE(PI_STATUS_REG, PI_STATUS_CLR_INTR);
    __OSGlobalIntMask |= OS_IM_PI;
}

static void __osLeoResume(void) {
    __OSEventState* event_state;
    OSMesgQueue* message_queue;
    s32 last;

    event_state = &__osEventStateTab[OS_EVENT_PI];
    message_queue = event_state->messageQueue;

    if ((message_queue == NULL) || MQ_IS_FULL(message_queue)) {
        return;
    }

    last = (message_queue->first + message_queue->validCount) % message_queue->msgCount;
    message_queue->msg[last] = event_state->message;
    message_queue->validCount++;

    if (message_queue->mtqueue->next != NULL) {
        __osEnqueueThread(&__osRunQueue, __osPopThread(&message_queue->mtqueue));
    }
}
