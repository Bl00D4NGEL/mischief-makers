#include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

glabel __rmonRCPrunning
    or      $v0, $zero, $zero
    lui     $t0, 0xA404
    lw      $t0, 0x10($t0)
    andi    $t0, $t0, 0x3
    bnez    $t0, isHalted
     nop
    ori     $v0, $v0, 0x1
isHalted:
    jr      $ra
     nop
.size __rmonRCPrunning, . - __rmonRCPrunning

glabel __rmonIdleRCP
    lui     $a0, 0xA404
    ori     $a0, $a0, 0x18
waitForDma:
    lw      $v0, 0x0($a0)
    bnez    $v0, waitForDma
     nop
    addiu   $a1, $zero, 0x82
    lui     $a0, 0xA404
    ori     $a0, $a0, 0x10
    sw      $a1, 0x0($a0)
awaitIdle:
    lui     $a0, 0xA404
    ori     $a0, $a0, 0x10
    lw      $v0, 0x0($a0)
    andi    $v0, $v0, 0x3
    beqz    $v0, awaitIdle
     nop
    jr      $ra
     nop
.size __rmonIdleRCP, . - __rmonIdleRCP

glabel __rmonStepRCP
    lui     $a0, 0xA404
    ori     $a0, $a0, 0x10
    addiu   $a1, $zero, 0xC5
    sw      $a1, 0x0($a0)
    b       awaitIdle
     nop
.size __rmonStepRCP, . - __rmonStepRCP

glabel __rmonRunRCP
    lui     $a0, 0xA430
    ori     $a0, $a0, 0x0C
    addiu   $a1, $zero, 0x2
    sw      $a1, 0x0($a0)
    lui     $a0, 0xA404
    ori     $a0, $a0, 0x10
    addiu   $a1, $zero, 0x125
    sw      $a1, 0x0($a0)
    jr      $ra
     nop
.size __rmonRunRCP, . - __rmonRunRCP
