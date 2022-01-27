

.data

.bss
	.lcomm BUF 16384
.text

format_num:  // format value in r0 to BUF in decimal
    // input: r0
    // output: (data in BUF), r5 (index updated)
    // clobbers r0, r1, r2

1:
    mov w1, #10
    udiv w2, w0, w1

    // get remainder to r1
    smsubl x1,w1,w2,x0 

    add w1, w1, #'0'	// make it ascii number

    strb w1, [x4, x5]
    add x5, x5, #1

    mov x0, x2
    cbnz x0, 1b

    ret

reverse_bytes:
    // input: r5 bytes in BUF (r4) to handle
    // output: (data in BUF)
    // clobbers r0, r1, r2, r3

    mov x2, x4		// head
    add x3, x4, x5
    sub x3, x3, #1		// tail
1:
    cmp x2, x3
    b.ge done
    // swap [r2] and [r3]
    ldrb w0, [x2]
    ldrb w1, [x3]
    strb w1, [x2]
    strb w0, [x3]
    add x2, x2, #1
    sub x3, x3, #1
    b 1b
done:
    ret

update_counters:
    // input: r5 bytes in BUF (r4) to handle
    // output: r6, r8, r9
    // clobbers r0, r1, r2, r3

    // update total byte count
    add x6, x6, x5

    mov x1, #0 		// index
    mov w3, #0 		// temp
1:
    ldrb w0, [x4, x1]

    // increase linefeed count for each \n
    cmp w0, #'\n'
    b.ne 2f
    add x7, x7, #1
    mov w3, #1		// is whitespace
    b 3f
2:
    // check for other whitespace (' ', \t)
    cmp w0, #' '
    b.ne 4f
    mov w3, #1
4:
    cmp w0, #'\t'
    b.ne 3f
    mov w3, #1
3:
    cmp w0, #11 // vertical tab
    b.ne 5f
    mov w3, #1
5:
    cmp w0, #12 // form feed
    b.ne 6f
    mov w3, #1
6:
    cmp w0, #13 // carriage return
    b.ne 7f
    mov w3, #1
7:
    // if r3==0 && r10==1
    // then count as a new word
    cmp w3, w10
    cset x2, lt		// x2 is 1 or 0
    add x9, x9, x2

    mov w10, w3
    mov w3, #0

    add x1, x1, #1
    cmp x1, x5
    b.lt 1b
    
    ret

.globl _start
_start:

    // global register variables
    // r4 BUF start address
    // r5 index (to BUF)
    // r6 byte count
    // (w8 reserved for syscall no)
    // x7 linefeed count
    // r9 word count
    // r10 set to 1 when whitespace, otherwise 0

    
    // initialization
    ldr x4, =BUF
    mov x5, #0
    mov x6, #0
    mov x7, #0
   // skip r8
    mov x9, #0
    mov x10, #1		// make the 1st character to trigger word counter

read:
   // read a block of data
    mov x0, #0		// fd (stdin)
    mov x1, x4		// buffer
    mov x2, #16384
    mov x8, #63  // syscall 63 - read
    svc #0
    
    cmp x0, #0
    b.ge 1f
    mov x0, #1 // negative - exit with status 1
    b exit
1:
    b.ne 2f
    mov x0, #0 // zero - EOF - print result
    b print
2:
    mov x5,x0 // bytes read

    bl update_counters
    
    // read more
    b read

print:
    // print values

    mov x5, #0 // re-use buffer

    mov w1, #'\n'  // start with newline.. it'll be reversed
    strb w1, [x4, x5]
    add x5, x5, #1

    // format byte count
    mov x0, x6
    bl format_num
    
    mov w1, #' '	// space between numbers
    strb w1, [x4, x5]
    add x5, x5, #1

    mov x0, x9		// word count
    bl format_num
    
    mov w1, #' '	// space
    strb w1, [x4, x5]
    add x5, x5, #1
    
    mov x0, x7
    bl format_num

    bl reverse_bytes
    
    // write output
    mov x0, #1 // fd
    mov x1, x4
    mov x2, x5
    mov w8, #64  // syscall 64 - write
    svc #0

    mov x0, #0 // exit code
exit:
    mov w8, #93  // syscall 93 - exit
    svc #0
