#include <stdint.h>
#include <stdio.h>

uint32_t crc32(const char *str)
{
    uint32_t crc = 0xFFFFFFFF;
    asm("1: mov al, [%[P]];"
        "cmp al, 0;"
        "jz 2f;"
        "crc32 %[CRC], al;"
        "inc %[P];"
        "jmp 1b;"
        "2:"
        : [CRC] "+r"(crc), [P] "+r"(str)
        : "m"(*(const char(*)[])str)
        : "rax" );
    return crc ^ 0xFFFFFFFF;
}

int main()
{
    puts("Testing some inline assembly");
    printf("crc32(aaa)=%X\n", crc32("aaa"));
    printf("crc32(Test)=%X\n", crc32("Test"));
    printf("crc32(TestTest)=%X\n", crc32("TestTest"));
}
