#include <PR/os_internal.h>
#include <PR/rdb.h>

rdbPacket* __osRdb_IP6_Data;
u32 __osRdb_IP6_Size;
u32 __osRdb_IP6_Ct;
u32 __osRdb_IP6_CurWrite;
u32 __osRdb_IP6_CurSend;

void osInitRdb(u8* send_buf, u32 send_size) {
    u32 mask;

    send_size /= 4;

    if (((u32)send_buf & 3) != 0) {
        send_buf = (u8*)(((u32)send_buf & 3) + 4);
        send_size--;
    }

    mask = __osDisableInt();

    __osRdb_IP6_Data = (rdbPacket*)send_buf;
    __osRdb_IP6_Size = send_size;
    __osRdb_IP6_Ct = 0;
    __osRdb_IP6_CurWrite = 0;
    __osRdb_IP6_CurSend = 0;

    __osRestoreInt(mask);
}
