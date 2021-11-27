.text
.globl _start
_start:
    // return argc as exit status
    
    pop ebx;
    mov eax, 1;
    int 0x80;
