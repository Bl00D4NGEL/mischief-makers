#include <PR/os_internal.h>
#include <PR/rcp.h>
#include <PR/rdb.h>

OSThread __osThreadSave;

extern OSThread* __osRunningThread;
extern u32 __osRdb_IP6_Empty;

static u8 buffer[12];
static u32 numChars = 0;

static u32 string_to_u32(u8* str) {
    u32 value;

    value = ((str[0] & 0xFF) << 0x18);
    value |= ((str[1] & 0xFF) << 0x10);
    value |= ((str[2] & 0xFF) << 0x8);
    value |= (str[3] & 0xFF);

    return value;
}

static void send_packet(u8* str, u32 count) {
    rdbPacket packet;
    u32 index;

    packet.type = 0xC;
    packet.length = count;

    for (index = 0; index < count; index++) {
        packet.buf[index] = str[index];
    }
    *(vu32*)RDB_BASE_REG = *(u32*)&packet;
}

static void clear_IP6(void) {
    while (!(__osGetCause() & CAUSE_IP6)) {
    }
    *(vu32*)RDB_READ_INTR_REG = 0;

    while (__osGetCause() & CAUSE_IP6) {
    }
}

static void send(u8* str, u32 count) {
    u32 packet_count;
    u32 index = 0;
    u32 wait_for_last_ip6;

    if (!__osRdb_IP6_Empty) {
        clear_IP6();
        wait_for_last_ip6 = FALSE;
    }
    else {
        wait_for_last_ip6 = TRUE;
    }

    while (count != 0) {
        packet_count = (count < 3) ? count : 3;
        send_packet(str + index, packet_count);
        count -= packet_count;
        index += packet_count;
        if (count != 0) {
            clear_IP6();
        }
    }

    if (wait_for_last_ip6) {
        clear_IP6();
    }
}

void kdebugserver(rdbPacket packet) {
    u32 index;
    u32 length;
    u8* address;

    for (index = 0; index < 3; index++) {
        buffer[numChars++] = packet.buf[index];
    }

    if (buffer[0] == 2) {
        send((u8*)&__osRunningThread->context, sizeof(__OSThreadContext));
        numChars = 0;
    }
    else if (numChars >= 9 && buffer[0] == 1) {
        address = (u8*)string_to_u32(&buffer[1]);
        length = string_to_u32(&buffer[5]);
        send(address, length);
        numChars = 0;
    }
}
