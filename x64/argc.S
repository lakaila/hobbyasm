.text
.globl _start
_start:
    // return argc as exit status
    
    pop rdi;
    mov rax, 60;
    syscall;
