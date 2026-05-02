#include "PR/os_internal.h"
#include "PRinternal/viint.h"

__OSViContext* __osViGetCurrentContext(void) {
    return __osViCurr;
}
