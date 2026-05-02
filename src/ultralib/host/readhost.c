#include <PR/os_internal.h>
#include <PR/rdb.h>
#include <PRinternal/macros.h>

static int readHostInitialized = FALSE;
static OSMesgQueue readHostMesgQueue ALIGNED(0x8);
static OSMesg readHostMesgBuf[1];

u32 __osRdb_Read_Data_Buf;
u32 __osRdb_Read_Data_Ct;

void osReadHost(void* dram_addr, u32 nbytes) {
    char buffer[4];
    u32 sent = 0;

    if (!readHostInitialized) {
        osCreateMesgQueue(&readHostMesgQueue, readHostMesgBuf, ARRLEN(readHostMesgBuf));
        osSetEventMesg(OS_EVENT_RDB_READ_DONE, &readHostMesgQueue, NULL);
        readHostInitialized = TRUE;
    }

    __osRdb_Read_Data_Buf = (u32)dram_addr;
    __osRdb_Read_Data_Ct = nbytes;

    while (sent == 0) {
        sent += __osRdbSend(buffer, 1, RDB_TYPE_GtoH_READY_FOR_DATA);
    }

    osRecvMesg(&readHostMesgQueue, NULL, OS_MESG_BLOCK);
    return;
}
