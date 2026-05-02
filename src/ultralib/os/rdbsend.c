#include "PR/os_internal.h"
#include "PR/rdb.h"
#include "PR/R4300.h"

extern rdbPacket* __osRdb_IP6_Data;
extern u32 __osRdb_IP6_Size;
extern u32 __osRdb_IP6_Ct;
extern u32 __osRdb_IP6_CurWrite;

u32 __osRdb_IP6_Empty = 1;

u32 __osRdbSend(u8* buffer, u32 size, u32 type) {
    rdbPacket* packet_pointer;
    rdbPacket packet;
    u32 length;
    u32 index;
    u32 input_count = 0;
    u32 need_first = 0;
    u32 mask;
    u32 sent = 0;

    mask = __osDisableInt();

    if (__osRdb_IP6_Empty != 0) {
        __osRdb_IP6_Empty = 0;
        packet.type = type;

        length = (size < sizeof(packet.buf)) ? size : sizeof(packet.buf);

        packet.length = length;

        index = 0;
        while (index < length) {
            packet.buf[index++] = buffer[input_count++];
        }

        size -= length;
        sent = length;
        need_first = 1;
    }

    while ((size != 0) && (__osRdb_IP6_Ct < __osRdb_IP6_Size)) {
        length = (size < sizeof(packet_pointer->buf)) ? size : sizeof(packet_pointer->buf);

        packet_pointer = &__osRdb_IP6_Data[__osRdb_IP6_CurWrite];

        packet_pointer->type = type;
        packet_pointer->length = length;

        index = 0;
        while (index < length) {
            packet_pointer->buf[index++] = buffer[input_count++];
        }

        __osRdb_IP6_CurWrite++;
        if (__osRdb_IP6_CurWrite >= __osRdb_IP6_Size) {
            __osRdb_IP6_CurWrite = 0;
        }

        __osRdb_IP6_Ct++;
        size -= length;
        sent += length;
    }

    if (need_first) {
        *(u32*)RDB_BASE_REG = *(u32*)&packet;
    }
    __osRestoreInt(mask);

    return sent;
}
