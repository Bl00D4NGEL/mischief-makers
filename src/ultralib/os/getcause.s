#include "PR/R4300.h"
#include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

glabel __osGetCause
    mfc0    $v0, C0_CAUSE
    jr      $ra
     nop
.size __osGetCause, . - __osGetCause
