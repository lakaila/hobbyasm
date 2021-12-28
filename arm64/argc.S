.text
.globl _start
_start:
    // return argc as exit status
    ldr x0, [sp]
    mov w8, #93
    svc #0
