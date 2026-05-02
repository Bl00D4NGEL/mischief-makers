.include "macro.inc"

.set noat
.set noreorder
.set gp=64

.section .rdata, "a"

dlabel __libm_qnan_f
.word 0x7F810000
.float 0.0
.float 0.0
.float 0.0
.size __libm_qnan_f, . - __libm_qnan_f
