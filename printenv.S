
.text

strlen:
    // input: r1
    // output: r2
    // clobbers: r0
    push {lr}
    // Note: r0==NULL is not checked
    mov r2, #0
1:
    ldrb r0, [r1, r2]
    cmp r0, #0
    addne r2, #1
    bne 1b

    pop {pc}

writeln:
    // input: r1 (str), r2 (strlen)
    // output:
    // clobbers: r1, r3, r7
    push {lr}

    // append linefeed Note: this will modify the data area!
    mov r3, #'\n'
    strb r3, [r1, r2]
    add r2, #1

    mov r0, #1 // fd
    mov r7, #4  // syscall 4 - write
    svc #0
    // TODO: should check error?
    pop {pc}

.globl _start
_start:

    // Start looking into stack upwards
    // there is argc followed by argv[]

    ldr r4, [sp] // this is argc
    add r4, #2 // skip the last arg and null terminator

    mov r5, sp
    add r5, r4, LSL #2

1:
    ldr r1, [r5], #4
    cmp r1, #0
    beq done		// if null terminator, exit
    bl strlen
    bl writeln
    b 1b
done:

    mov r0, #0 // exit code
exit:
    mov r7, #1  // syscall 1 - exit
    svc #0
