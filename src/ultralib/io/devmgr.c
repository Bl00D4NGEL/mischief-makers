#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "piint.h"

void __osDevMgrMain(void* args) {
    OSIoMesg* message_block;
    OSMesg event_mesg;
    OSMesg dummy_mesg;
    s32 result;
    OSDevMgr* device_manager;
    s32 send_message = 0;

    device_manager = (OSDevMgr*)args;
    message_block = NULL;
    result = 0;

    while (TRUE) {
        osRecvMesg(device_manager->cmdQueue, (OSMesg)&message_block, OS_MESG_BLOCK);

        if ((message_block->piHandle != NULL) && (message_block->piHandle->type == DEVICE_TYPE_64DD) &&
            ((message_block->piHandle->transferInfo.cmdType == LEO_CMD_TYPE_0) ||
             (message_block->piHandle->transferInfo.cmdType == LEO_CMD_TYPE_1))) {
            __OSBlockInfo* block_info;
            __OSTranxInfo* transfer_info;

            transfer_info = &message_block->piHandle->transferInfo;
            block_info = &transfer_info->block[transfer_info->blockNum];
            transfer_info->sectorNum = -1;

            if (transfer_info->transferMode != LEO_SECTOR_MODE) {
                block_info->dramAddr = (void*)((u32)block_info->dramAddr - block_info->sectorSize);
            }

            if ((transfer_info->transferMode == LEO_TRACK_MODE) &&
                (message_block->piHandle->transferInfo.cmdType == LEO_CMD_TYPE_0)) {
                send_message = 1;
            }
            else {
                send_message = 0;
            }

            osRecvMesg(device_manager->acsQueue, &dummy_mesg, OS_MESG_BLOCK);
            __osResetGlobalIntMask(OS_IM_PI);
            osEPiRawWriteIo(message_block->piHandle, LEO_BM_CTL, transfer_info->bmCtlShadow | 0x80000000);

        read_block:
            osRecvMesg(device_manager->evtQueue, &event_mesg, OS_MESG_BLOCK);
            transfer_info = &message_block->piHandle->transferInfo;
            block_info = &transfer_info->block[transfer_info->blockNum];

            if (block_info->errStatus == LEO_ERROR_29) {
                u32 status;

                osEPiRawWriteIo(message_block->piHandle, LEO_BM_CTL, transfer_info->bmCtlShadow | LEO_BM_CTL_RESET);
                osEPiRawWriteIo(message_block->piHandle, LEO_BM_CTL, transfer_info->bmCtlShadow);
                osEPiRawReadIo(message_block->piHandle, LEO_STATUS, &status);

                if (status & LEO_STATUS_MECHANIC_INTERRUPT) {
                    osEPiRawWriteIo(message_block->piHandle, LEO_BM_CTL,
                                    transfer_info->bmCtlShadow | LEO_BM_CTL_CLR_MECHANIC_INTR);
                }

                block_info->errStatus = LEO_ERROR_4;
                IO_WRITE(PI_STATUS_REG, PI_CLR_INTR);
                __osSetGlobalIntMask(OS_IM_PI | SR_IBIT4);
            }

            osSendMesg(message_block->hdr.retQueue, message_block, OS_MESG_NOBLOCK);

            if ((send_message == 1) && (message_block->piHandle->transferInfo.block[0].errStatus == LEO_ERROR_GOOD)) {
                send_message = 0;
                goto read_block;
            }

            osSendMesg(device_manager->acsQueue, NULL, OS_MESG_NOBLOCK);
            if (message_block->piHandle->transferInfo.blockNum == 1) {
                osYieldThread();
            }
        }
        else {
            switch (message_block->hdr.type) {
                case OS_MESG_TYPE_DMAREAD:
                    osRecvMesg(device_manager->acsQueue, &dummy_mesg, OS_MESG_BLOCK);
                    result = device_manager->dma(OS_READ, message_block->devAddr, message_block->dramAddr,
                                                 message_block->size);
                    break;

                case OS_MESG_TYPE_DMAWRITE:
                    osRecvMesg(device_manager->acsQueue, &dummy_mesg, OS_MESG_BLOCK);
                    result = device_manager->dma(OS_WRITE, message_block->devAddr, message_block->dramAddr,
                                                 message_block->size);
                    break;

                case OS_MESG_TYPE_EDMAREAD:
                    osRecvMesg(device_manager->acsQueue, &dummy_mesg, OS_MESG_BLOCK);
                    result = device_manager->edma(message_block->piHandle, OS_READ, message_block->devAddr,
                                                  message_block->dramAddr, message_block->size);
                    break;

                case OS_MESG_TYPE_EDMAWRITE:
                    osRecvMesg(device_manager->acsQueue, &dummy_mesg, OS_MESG_BLOCK);
                    result = device_manager->edma(message_block->piHandle, OS_WRITE, message_block->devAddr,
                                                  message_block->dramAddr, message_block->size);
                    break;

                case OS_MESG_TYPE_LOOPBACK:
                    osSendMesg(message_block->hdr.retQueue, message_block, OS_MESG_NOBLOCK);
                    result = -1;
                    break;

                default:
                    result = -1;
                    break;
            }

            if (result == 0) {
                osRecvMesg(device_manager->evtQueue, &event_mesg, OS_MESG_BLOCK);
                osSendMesg(message_block->hdr.retQueue, message_block, OS_MESG_NOBLOCK);
                osSendMesg(device_manager->acsQueue, NULL, OS_MESG_NOBLOCK);
            }
        }
    }
}
