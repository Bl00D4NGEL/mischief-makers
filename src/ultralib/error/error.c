#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include <PR/ultralog.h>
#include <PRinternal/macros.h>
#include <compiler/ido/stdarg.h>

void __osLogWrite(OSLog* log, s16 code, s16 num_args, va_list args);

static void __osDefaultHandler(s16 code, s16 num_args, ...);

static u32 errorLogData[19] ALIGNED(0x8);
static OSLog errorLog ALIGNED(0x8) = {
    OS_ERROR_MAGIC,
    sizeof(errorLogData),
    errorLogData,
    0,
    0,
};

OSErrorHandler __osErrorHandler = __osDefaultHandler;

static void __osDefaultHandler(s16 code, s16 num_args, ...) {
    va_list args;

    va_start(args, num_args);
    __osLogWrite(&errorLog, code, num_args, args);
    osFlushLog(&errorLog);
    va_end(args);
}
