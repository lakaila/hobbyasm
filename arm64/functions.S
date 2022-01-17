/*
 Some examples how to mix C and asm.

 Warning: a signal handler may use stack - therefore update (decrese) SP always
 before using it.

*/
.text
.globl func1
.globl func2
.globl func3
.globl func4

func1:
    // int32_t func1(void);
    mov w0, #55
    ret

func2:
    // void func2(void);

    // store frame record
    stp x29, x30, [sp, -0x10]!
    mov x29, sp		// make FP to point frame record
    
    // these registers can be corrupted

    // x0-x7 parameter input/ result return
    mov x0, #11
    mov x1, #11
    mov x2, #11
    mov x3, #11
    mov x4, #11
    mov x5, #11
    mov x6, #11
    mov x7, #11
    mov x8, #11 // indirect result location (what that means)
   
    mov x9, #22
    mov x10, #22
    mov x11, #22
    mov x12, #22
    mov x13, #22
    mov x14, #22
    mov x15, #22

    // x16 IP0 - unsure when can be corrupted
    // x17 IP1 - unsure when can be corrupted
    // x18 possible platform register - unsure when can be corrupted

    // x19-x28 must be preserved

    // x29 is the fame pointer
    // x30 is the link register
    // x31 is the SP

    ldp x29, x30, [sp], 0x10

    ret

func3:
    // int32_t func3(int32_t);
    lsl w0, w0, #1
    ret

func4:
    // int32_t func4(int32_t, int32_t);
    stp x29, x30, [sp, -0x10]!
    mov x29, sp

    // this time we know w1 is not corrupted, so not saved before call
    bl func3
    add w0, w0, w1

    ldp x29, x30, [sp], 0x10
    ret
