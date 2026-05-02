.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .text, "ax"

glabel sqrtf
/* A6310 800A5710 46006004 */  sqrt.s     $f0, $f12
/* A6314 800A5714 03E00008 */  jr         $ra
/* A6318 800A5718 00000000 */   nop
.size sqrtf, . - sqrtf
/* A631C 800A571C 00000000 */  nop
