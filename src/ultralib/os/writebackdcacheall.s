#include "PR/R4300.h"
#include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/* Handwritten function */
glabel osWritebackDCacheAll
    lui     $t0, (K0BASE >> 16)
    addiu   $t2, $zero, DCACHE_SIZE
    addu    $t1, $t0, $t2
    addiu   $t1, $t1, -DCACHE_LINESIZE
1:
    cache   (C_IWBINV | CACH_PD), 0($t0)
    sltu    $at, $t0, $t1
    bnez    $at, 1b
     addiu  $t0, $t0, DCACHE_LINESIZE
    jr      $ra
     nop
endlabel osWritebackDCacheAll
