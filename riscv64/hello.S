.text
.globl _start
_start:
# write(0, (ptr), 6)
    li a0, 0
    lla a1, msg
    li a2, 6
    li a7, 64
    ecall

# exit with code 0
    li a0, 0
    li a7, 93
    ecall
.data
msg: .ascii "Hello\n"
