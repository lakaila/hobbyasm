#include <stdint.h>
#include <stdio.h>

int32_t add_asm(int32_t a, int32_t b)
{
    asm("add %w[A], %w[A], %w[B];"
        : [A] "+r"(a)
        : [B] "r"(b) :);
    return a;
}

int32_t add_c(int32_t a, int32_t b)
{
    return a + b;
}

size_t strlen_asm(const char *str)
{
    size_t len = 0;
    asm("b 2f;"
        "1: add %[RET], %[RET], #1;"
        "2: ldrb w1, [%[P]], #1;"
        "cmp w1, wzr;"
        "bne 1b;"
        : [RET] "+r"(len)
        : [P] "r"(str), "m"(*(const char(*)[])str)
        : "w1");
    return len;
}

size_t strlen_asm2(const char *str)
{
    size_t len = 0;
    asm("mov %[RET], %[P];"
        "1: ldrb w1, [%[P]], #1;"
        "cmp w1, wzr;"
        "bne 1b;"
        "sub %[RET], %[P], %[RET];"
        "sub %[RET], %[RET], #1;"
        : [RET] "+r"(len)
        : [P] "r"(str), "m"(*(const char(*)[])str)
        : "w1");
    return len;
}

uint32_t crc32(const char *str)
{
    uint32_t crc = 0xFFFFFFFF;
    asm("b 2f;"
        "1: crc32b %w[CRC], %w[CRC], w1;"
        "2: ldrb w1, [%[P]], #1;"
        "cmp w1, wzr;"
        "bne 1b;"
        : [CRC] "+r"(crc)
        : [P] "r"(str), "m"(*(const char(*)[])str)
        : "w1");
    return crc ^ 0xFFFFFFFF;
}

uint32_t crc32c(const char *str)
{
    uint32_t crc = 0xFFFFFFFF;
    asm("b 2f;"
        "1: crc32cb %w[CRC], %w[CRC], w1;"
        "2: ldrb w1, [%[P]], #1;"
        "cmp w1, wzr;"
        "bne 1b;"
        : [CRC] "+r"(crc)
        : [P] "r"(str), "m"(*(const char(*)[])str)
        : "w1");
    return crc ^ 0xFFFFFFFF;
}

int main()
{
    puts("Testing some inline assembly");
    printf("1000+567=%d\n", add_asm(1000, 567));
    printf("1000+567=%d\n", add_c(1000, 567));
    printf("strlen=%zu\n", strlen_asm2("sadaadads"));
    printf("strlen=%zu\n", strlen_asm2(""));
    printf("strlen=%zu\n", strlen_asm2("d aidaidadiadiajdoaijoaiod"));
    printf("crc32(aaa)=%X crc32c=%x\n", crc32("aaa"), crc32c("aaa"));
    printf("crc32(Test)=%X crc32c=%x\n", crc32("Test"), crc32c("Test"));
    printf("crc32(1234567890)=%X crc32c=%x\n", crc32("1234567890"), crc32c("1234567890"));
    printf("crc32(The quick..)=%X\n", crc32("The quick brown fox jumps over the lazy dog"));
}