#include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

glabel osSetIntMask
    // Read the current status register and recover the previous CPU mask
    mfc0    $t4, $12
    andi    $v0, $t4, 0xFF01

    // Fold disabled global CPU interrupt bits into the returned mask
    lui     $t0, %hi(__OSGlobalIntMask)
    addiu   $t0, $t0, %lo(__OSGlobalIntMask)
    lw      $t3, 0x0($t0)
    addiu   $at, $zero, -1
    xor     $t0, $t3, $at
    andi    $t0, $t0, 0xFF00
    or      $v0, $v0, $t0

    // Read MI_INTR_MASK_REG and recover the previous RCP mask
    lui     $t2, (0xA430000C >> 16)
    lw      $t2, (0xA430000C & 0xFFFF)($t2)
    beqz    $t2, .Lrcp_mask_done
     srl    $t1, $t3, 16

    addiu   $at, $zero, -1
    xor     $t1, $t1, $at
    andi    $t1, $t1, 0x3F
    or      $t2, $t2, $t1

.Lrcp_mask_done:
    sll     $t2, $t2, 16
    or      $v0, $v0, $t2

    // Convert requested RCP mask bits to MI_INTR_MASK_REG set/clear bits
    lui     $at, (0x3F0000 >> 16)
    and     $t0, $a0, $at
    and     $t0, $t0, $t3
    srl     $t0, $t0, 15
    lui     $t2, %hi(__osRcpImTable)
    addu    $t2, $t2, $t0
    lhu     $t2, %lo(__osRcpImTable)($t2)
    lui     $at, (0xA430000C >> 16)
    sw      $t2, (0xA430000C & 0xFFFF)($at)

    // Apply requested CPU mask bits through the global interrupt mask
    andi    $t0, $a0, 0xFF01
    andi    $t1, $t3, 0xFF00
    and     $t0, $t0, $t1
    lui     $at, (0xFFFF00FF >> 16)
    ori     $at, $at, (0xFFFF00FF & 0xFFFF)
    and     $t4, $t4, $at
    or      $t4, $t4, $t0

    mtc0    $t4, $12
    nop
    nop
    jr      $ra
     nop
.size osSetIntMask, . - osSetIntMask

.section .rodata, "a"
.balign 16

// Lookup table from MI interrupt status bits to MI interrupt mask set/clear bits
dlabel __osRcpImTable
    .short 0x0555
    .short 0x0556
    .short 0x0559
    .short 0x055A
    .short 0x0565
    .short 0x0566
    .short 0x0569
    .short 0x056A
    .short 0x0595
    .short 0x0596
    .short 0x0599
    .short 0x059A
    .short 0x05A5
    .short 0x05A6
    .short 0x05A9
    .short 0x05AA
    .short 0x0655
    .short 0x0656
    .short 0x0659
    .short 0x065A
    .short 0x0665
    .short 0x0666
    .short 0x0669
    .short 0x066A
    .short 0x0695
    .short 0x0696
    .short 0x0699
    .short 0x069A
    .short 0x06A5
    .short 0x06A6
    .short 0x06A9
    .short 0x06AA
    .short 0x0955
    .short 0x0956
    .short 0x0959
    .short 0x095A
    .short 0x0965
    .short 0x0966
    .short 0x0969
    .short 0x096A
    .short 0x0995
    .short 0x0996
    .short 0x0999
    .short 0x099A
    .short 0x09A5
    .short 0x09A6
    .short 0x09A9
    .short 0x09AA
    .short 0x0A55
    .short 0x0A56
    .short 0x0A59
    .short 0x0A5A
    .short 0x0A65
    .short 0x0A66
    .short 0x0A69
    .short 0x0A6A
    .short 0x0A95
    .short 0x0A96
    .short 0x0A99
    .short 0x0A9A
    .short 0x0AA5
    .short 0x0AA6
    .short 0x0AA9
    .short 0x0AAA
enddlabel __osRcpImTable
.size __osRcpImTable, . - __osRcpImTable
