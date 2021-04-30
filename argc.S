.text
.globl _start
_start:
    // return argc as exit status
    ldr r0, [sp]
    mov r7, #1
    svc #0
