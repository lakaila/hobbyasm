.text
.globl _start
_start:
# return argc as exit status
    lw a0, 0(sp) 
    li a7, 93
    ecall
