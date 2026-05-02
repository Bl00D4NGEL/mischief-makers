#include "PR/R4300.h"
#include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

/* Handwritten function */
glabel osInvalDCache
    blez    $a1, 3f
     nop
    addiu   $t3, $zero, DCACHE_SIZE
    sltu    $at, $a1, $t3
    beqz    $at, 4f
     nop
    or      $t0, $a0, $zero

    addu    $t1, $a0, $a1
    sltu    $at, $t0, $t1
    beqz    $at, 3f
     nop
    addiu   $t1, $t1, -DCACHE_LINESIZE

    andi    $t2, $t0, DCACHE_LINEMASK
    beqz    $t2, 1f
     nop

    subu    $t0, $t0, $t2
    cache   (C_HWBINV | CACH_PD), 0($t0)
    sltu    $at, $t0, $t1
    beqz    $at, 3f
     nop

    addiu   $t0, $t0, DCACHE_LINESIZE
1:
    andi    $t2, $t1, DCACHE_LINEMASK
    beqz    $t2, 2f
     nop

    subu    $t1, $t1, $t2
    cache   (C_HWBINV | CACH_PD), DCACHE_LINESIZE($t1)
    sltu    $at, $t1, $t0
    bnez    $at, 3f
     nop

2:
    cache   (C_HINV | CACH_PD), 0($t0)
    sltu    $at, $t0, $t1
    bnez    $at, 2b
     addiu  $t0, $t0, DCACHE_LINESIZE
3:
    jr      $ra
     nop

4:
    lui     $t0, (K0BASE >> 16)
    addu    $t1, $t0, $t3
    addiu   $t1, $t1, -DCACHE_LINESIZE
5:
    cache   (C_IINV | CACH_PD), 0($t0)
    sltu    $at, $t0, $t1
    bnez    $at, 5b
     addiu  $t0, $t0, DCACHE_LINESIZE

    jr      $ra
     nop
endlabel osInvalDCache
