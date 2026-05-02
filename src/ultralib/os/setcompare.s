#include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

glabel __osSetCompare
    mtc0    $a0, $11
    jr      $ra
     nop
.size __osSetCompare, . - __osSetCompare
