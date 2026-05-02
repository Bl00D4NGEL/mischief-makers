#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include "osint.h"
#include "PRinternal/viint.h"

OSTime osGetTime(void) {
    u32 tmp_time;
    u32 elapsed_count;
    OSTime current_time;
    register u32 save_mask;

    if (!__osViDevMgr.active) {
        __osError(ERR_OSGETTIME, 0);
        return 0;
    }

    save_mask = __osDisableInt();
    tmp_time = osGetCount();
    elapsed_count = tmp_time - __osBaseCounter;
    current_time = __osCurrentTime;
    __osRestoreInt(save_mask);
    return current_time + elapsed_count;
}
