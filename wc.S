

.data

.bss
	.lcomm BUF 1024
.text

format_num:  // format value in r0 to BUF in decimal
    push {lr}

    ldr r4, =BUF	// pointer
1:
    mov r1, #10
    udiv r2, r0, r1

    // get remainder to r1
    mls r1,r1,r2,r0 

    add r1, #'0'	// make it ascii number

    strb r1, [r4, r5]
    add r5, #1

    movs r0, r2
    bne 1b

    pop {pc}

reverse_bytes:
    push {lr}
    ldr r4, =BUF	// begin
    add r3, r4, r5
    sub r3, #1		// end
1:
    cmp r4, r3
    bge done
    // swap [r4] and [r3]
    ldrb r0, [r4]
    ldrb r1, [r3]
    strb r1, [r4]
    strb r0, [r3]
    add r4, #1
    sub r3, #1
    b 1b
done:
    pop {pc}

update_counters:
    // input: r5 bytes in BUF (r4) to handle
    // output: r6, r8, r9
    // clobbers r0, r1, r2, r3
    push {lr}

    // update total byte count
    add r6, r5

    mov r2, #1		// set to 1 when whitespace, otherwise 0
    mov r1, #0 		// index
    mov r3, #0 		// temp
1:
    ldrb r0, [r4, r1]

    // increase linefeed count for each \n
    cmp r0, #'\n'
    addeq r8, #1

    // check for whitespace (' ', \t \n)
    moveq r3, #1
    cmp r0, #' '
    moveq r3, #1
    cmp r0, #'\t'
    moveq r3, #1

    // if r3==0 && r2==1
    // then count as a new word
    cmp r3, r2
    addlt r9, #1

    // TODO: consider if a word is split between 2 read() calls (should make r2 a global var)

    mov r2, r3
    mov r3, #0

    add r1, #1
    cmp r1, r5
    blt 1b
    
    pop {pc}

.globl _start
_start:

    // global register variables
    // r4 BUF start address
    // r5 index (to BUF)
    // r6 byte count
    // (r7 reserved for syscall no)
    // r8 linefeed count
    // r9 word count
    
    // initialization
    ldr r4, =BUF
    mov r5, #0
    mov r6, #0
   // skip r7
    mov r8, #0
    mov r9, #0

read:
   // read a block of data
    mov r0, #0		// fd (stdin)
    ldr r1, =BUF	// pointer
    mov r2, #1024
    mov r7, #3  // syscall 3 - read
    svc #0
    
    cmp r0, #0
    movlt r0, #1 // negative - exit with status 1
    blt exit
    moveq r0, #0 // zero - EOF - print result
    beq print
    
    mov r5,r0 // bytes read

    bl update_counters
    
    // read more
    b read

print:
    // print values

    ldr r4, =BUF   // TODO: check if r4 accidentlly used, initialize just once
    mov r5, #0 // re-use buffer

    mov r1, #'\n'  // start with newline.. it'll be reversed
    strb r1, [r4, r5]
    add r5, #1

    // format byte count
    mov r0, r6
    bl format_num
    
    mov r1, #' '	// space between numbers
    strb r1, [r4, r5]
    add r5, #1

    mov r0, r9		// word count
    bl format_num
    
    mov r1, #' '	// space
    strb r1, [r4, r5]
    add r5, #1
    
    mov r0, r8
    bl format_num

    bl reverse_bytes
    
    // write output
    mov r0, #1 // fd
    ldr r1, =BUF
    mov r2, r5
    mov r7, #4  // syscall 4 - write
    svc #0

    mov r0, #0 // exit code
exit:
    mov r7, #1  // syscall 1 - exit
    svc #0
