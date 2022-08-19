#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
        "b.ne 1b;"
        : [RET] "+r"(len), [P] "+r"(str)
        : "m"(*(const char(*)[])str)
        : "w1");
    return len;
}

size_t strlen_asm2(const char *str)
{
    size_t len = 0;
    asm("mov %[RET], %[P];"
        "1: ldrb w1, [%[P]], #1;"
        "cmp w1, wzr;"
        "b.ne 1b;"
        "sub %[RET], %[P], %[RET];"
        "sub %[RET], %[RET], #1;"
        : [RET] "+r"(len), [P] "+r"(str)
        : "m"(*(const char(*)[])str)
        : "w1");
    return len;
}

uint32_t crc32(const char *str)
{
    uint32_t crc = 0xFFFFFFFF;
    asm("b 2f;"
        "1: crc32b %w[CRC], %w[CRC], w1;"
        "2: ldrb w1, [%[P]], #1;"
        "cbnz w1, 1b;"
        : [CRC] "+r"(crc), [P] "+r"(str)
        : "m"(*(const char(*)[])str)
        : "w1");
    return crc ^ 0xFFFFFFFF;
}

uint32_t crc32c(const char *str)
{
    uint32_t crc = 0xFFFFFFFF;
    asm("b 2f;"
        "1: crc32cb %w[CRC], %w[CRC], w1;"
        "2: ldrb w1, [%[P]], #1;"
        "cbnz w1, 1b;"
        : [CRC] "+r"(crc), [P] "+r"(str)
        : "m"(*(const char(*)[])str)
        : "w1");
    return crc ^ 0xFFFFFFFF;
}

uint32_t crc32bin(const char *ptr, size_t len)
{
    uint32_t crc = 0xFFFFFFFF;
    asm("cmp %[LEN], xzr;"
        "b 1f;"
        "2: subs %[LEN], %[LEN], #1;"
        "1: b.eq 3f;"
        "ldrb w1, [%[P]], #1;"
        "crc32b %w[CRC], %w[CRC], w1;"
        "b 2b;"
        "3:"
        : [CRC] "+r"(crc), [P] "+r"(ptr), [LEN] "+r"(len)
        : "m"(*(const char(*)[])ptr)
        : "w1");
    return crc ^ 0xFFFFFFFF;
}

uint32_t crc32bin_16(const char *ptr, size_t len)
{
    // process 16 bytes at a time (halfwords)
    // address must be aligned! size must match!
    uint32_t crc = 0xFFFFFFFF;
    asm("cmp %[LEN], xzr;"
        "b 1f;"
        "2: subs %[LEN], %[LEN], #2;"
        "1: b.eq 3f;"
        "ldrh w1, [%[P]], #2;"
        "crc32h %w[CRC], %w[CRC], w1;"
        "b 2b;"
        "3:"
        : [CRC] "+r"(crc), [P] "+r"(ptr), [LEN] "+r"(len)
        : "m"(*(const char(*)[])ptr)
        : "w1");
    return crc ^ 0xFFFFFFFF;
}

uint32_t crc32bin_32(const char *ptr, size_t len)
{
    // process 32 bytes at a time (words)
    // address must be aligned! size must match!
    uint32_t crc = 0xFFFFFFFF;
    asm("cmp %[LEN], xzr;"
        "b 1f;"
        "2: subs %[LEN], %[LEN], #4;"
        "1: b.eq 3f;"
        "ldr w1, [%[P]], #4;"
        "crc32w %w[CRC], %w[CRC], w1;"
        "b 2b;"
        "3:"
        : [CRC] "+r"(crc), [P] "+r"(ptr), [LEN] "+r"(len)
        : "m"(*(const char(*)[])ptr)
        : "w1");
    return crc ^ 0xFFFFFFFF;
}

uint32_t crc32bin_64(const char *ptr, size_t len)
{
    // process 64 bytes at a time (doublewords)
    // address must be aligned! size must match!
    uint32_t crc = 0xFFFFFFFF;
    asm("cmp %[LEN], xzr;"
        "b 1f;"
        "2: subs %[LEN], %[LEN], #8;"
        "1: b.eq 3f;"
        "ldr x1, [%[P]], #8;"
        "crc32x %w[CRC], %w[CRC], x1;"
        "b 2b;"
        "3:"
        : [CRC] "+r"(crc), [P] "+r"(ptr), [LEN] "+r"(len)
        : "m"(*(const char(*)[])ptr)
        : "w1");
    return crc ^ 0xFFFFFFFF;
}

uint32_t crc32opt(const char *ptr, size_t len)
{
    size_t len1 = (8-((size_t)ptr & 7)) & 7;
    if (len1 > len) {
	len1 = len;
    }
    size_t len2 = (len - len1) & 0xFFFFFFFFFFFFFF8;
    size_t len3 = len - len2 - len1;

    //printf("len %zu -> %zu, %zu, %zu\n", len, len1, len2, len3);
    //assert(len1+len2+len3==len);
    
    uint32_t crc = 0xFFFFFFFF;
    asm( // unaligned part (if ptr is not aligned)
	"cmp %[L1], xzr;"
        "b 1f;"
        "2: subs %[L1], %[L1], #1;"
        "1: b.eq 3f;"
        "ldrb w1, [%[P]], #1;"
        "crc32b %w[CRC], %w[CRC], w1;"
        "b 2b;"
        "3:" // then 64-bit aligned part
	"cmp %[L2], xzr;"
        "b 4f;"
        "5: subs %[L2], %[L2], #8;"
        "4: b.eq 6f;"
        "ldr x1, [%[P]], #8;"
        "crc32x %w[CRC], %w[CRC], x1;"
        "b 5b;"
        "6:" // finally possible remainder
	"cmp %[L3], xzr;"
        "b 7f;"
        "8: subs %[L3], %[L3], #1;"
        "7: b.eq 9f;"
        "ldrb w1, [%[P]], #1;"
        "crc32b %w[CRC], %w[CRC], w1;"
        "b 8b;"
        "9:"
        : [CRC] "+r"(crc), [P] "+r"(ptr), [L1] "+r"(len1), [L2] "+r"(len2), [L3] "+r"(len3)
        : "m"(*(const char(*)[])ptr)
        : "x1");
    return crc ^ 0xFFFFFFFF;
}

void breakpoint()
{
    asm("brk #55");
}

int main()
{
    puts("Testing some inline assembly");

    const char *fox = "The quick brown fox jumps over the lazy dog";
    //breakpoint();
    printf("1000+567=%d\n", add_asm(1000, 567));
    printf("1000+567=%d\n", add_c(1000, 567));
    printf("strlen=%zu\n", strlen_asm2("sadaadads"));
    printf("strlen=%zu\n", strlen_asm2(""));
    printf("strlen=%zu\n", strlen_asm2("d aidaidadiadiajdoaijoaiod"));
    printf("crc32c(aaa)=%X\n", crc32c("aaa"));
    printf("crc32(aaa)=%X\n", crc32("aaa"));
    printf("crc32(Test)=%X\n", crc32("Test"));
    printf("crc32(TestTest)=%X\n", crc32("TestTest"));
    printf("crc32c(Test)=%X\n", crc32c("Test"));
    printf("crc32bin(Test)=%X\n", crc32bin("Test", 4));
    printf("crc32(1234567890)=%X crc32c=%x\n", crc32("1234567890"), crc32c("1234567890"));
    printf("crc32(The quick..)=%X\n", crc32(fox));
    printf("crc32bin_16(Test)=%X\n", crc32bin_16("Test", 4));
    printf("crc32bin_32(Test)=%X\n", crc32bin_32("Test", 4));
    printf("crc32bin_16(TestTest)=%X\n", crc32bin_16("TestTest", 8));
    printf("crc32bin_32(TestTest)=%X\n", crc32bin_32("TestTest", 8));
    printf("crc32bin_64(TestTest)=%X\n", crc32bin_64("TestTest", 8));
    char *teststr = "TestTestTestTestTestTestTest";
    printf("crc32(teststr)=%X\n", crc32(teststr));
    printf("crc32opt(teststr,4)=%X\n", crc32opt(teststr,4));
    printf("crc32opt(teststr+4,4)=%X\n", crc32opt(teststr+4,4));
    printf("crc32opt(teststr+1,4)=%X\n", crc32opt(teststr+1,4));
    printf("crc32opt(teststr+7,4)=%X\n", crc32opt(teststr+7,4));
    printf("crc32opt(teststr,8)=%X\n", crc32opt(teststr,8));
    printf("crc32opt(teststr)=%X\n", crc32opt(teststr, strlen(teststr)));
    printf("crc32opt(The quick..)=%X\n", crc32opt(fox, strlen(fox)));
}
