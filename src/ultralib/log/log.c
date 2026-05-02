#include <compiler/ido/stdarg.h>
#include <PR/os.h>
#include <PR/os_internal.h>
#include <PR/rdb.h>
#include <PR/ultralog.h>
#include <PRinternal/macros.h>

static u32 __osLogOKtoWrite = TRUE;
static u32 __osLogInitialized = FALSE;

static OSMesgQueue __osLogDoneMsgQ ALIGNED(0x8);
static OSMesg __osLogMsgBuf;

void __osLogWrite(OSLog* log, s16 code, s16 num_args, va_list arg_ptr);

void osCreateLog(OSLog* log, u32* base, s32 byte_len) {
    log->magic = OS_LOG_MAGIC;
    log->base = base;
    log->len = byte_len;
    log->startCount = osGetCount();
    log->writeOffset = 0;
}

void osLogEvent(OSLog* log, s16 code, s16 num_args, ...) {
    va_list arg_ptr;

    if (num_args > OS_LOG_MAX_ARGS) {
        return;
    }

    va_start(arg_ptr, num_args);
    __osLogWrite(log, code, num_args, arg_ptr);
    va_end(arg_ptr);
}

void osFlushLog(OSLog* log) {
    s32 mask;
    u32 sent;
    u32 count;
    u32 subcount;
    u8* base;
    u8 d_count[3];

    if (!__osLogInitialized) {
        osCreateMesgQueue(&__osLogDoneMsgQ, &__osLogMsgBuf, 1);
        osSetEventMesg(OS_EVENT_RDB_LOG_DONE, &__osLogDoneMsgQ, NULL);
        __osLogInitialized = TRUE;
    }

    mask = __osDisableInt();
    __osLogOKtoWrite = FALSE;
    base = (u8*)log->base;
    count = log->writeOffset * 4;
    __osRestoreInt(mask);

    while (count != 0) {
        subcount = (count < 0x8000) ? count : 0x8000;

        d_count[0] = (subcount & 0xFF0000) >> 0x10;
        d_count[1] = (subcount & 0xFF00) >> 8;
        d_count[2] = subcount & 0xFF;

        sent = 0;
        while (sent < ARRLEN(d_count)) {
            sent += __osRdbSend(d_count + sent, ARRLEN(d_count) - sent, RDB_TYPE_GtoH_LOG_CT);
        }

        sent = 0;
        while (sent < subcount) {
            sent += __osRdbSend(base + sent, subcount - sent, RDB_TYPE_GtoH_LOG);
        }

        count -= subcount;
        base += subcount;
        osRecvMesg(&__osLogDoneMsgQ, NULL, OS_MESG_BLOCK);
    }

    mask = __osDisableInt();
    log->writeOffset = 0;
    __osLogOKtoWrite = TRUE;
    __osRestoreInt(mask);
}

void __osLogWrite(OSLog* log, s16 code, s16 num_args, va_list arg_ptr) {
    s32 index;
    u32 mask;
    u32 buffer[19];
    u32* buffer_ptr;
    OSLogItem* header;
    s32* args;
    s32* dest;
    s32 num_longs;

    buffer_ptr = buffer;
    header = (OSLogItem*)buffer;
    args = (s32*)(buffer + 3);
    num_longs = num_args + 3;
    mask = __osDisableInt();

    header->magic = log->magic;
    header->timeStamp = osGetCount() - log->startCount;
    header->argCount = num_args;
    header->eventID = code;

    for (index = 0; index < num_args; index++) {
        *args++ = va_arg(arg_ptr, s32);
    }

    if (__osLogOKtoWrite) {
        if ((log->writeOffset + num_longs) < (log->len >> 2)) {
            dest = (s32*)(log->base + log->writeOffset);
            for (index = 0; index < num_longs; index++) {
                *dest++ = *buffer_ptr++;
            }
            log->writeOffset += num_longs;
        }
        else {
            __osLogOKtoWrite = FALSE;
        }
    }

    __osRestoreInt(mask);
}
