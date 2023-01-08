
#include <stdio.h>
#include <stdint.h>

uint64_t csr_rdcycle(void)
{
    uint64_t val = 0;
    asm("rdcycle %[VAL];"
        : [VAL] "=r" (val) : : );
    return val;
}

uint64_t csr_rdtime(void)
{
    uint64_t val = 0;
    asm("rdtime %[VAL];"
        : [VAL] "=r" (val) : : );
    return val;
}

int main()
{
    puts("inline hello!");

    printf("rdcycle: %llu\n", csr_rdcycle());
    printf("rdtime: %llu\n", csr_rdcycle());
    printf("rdcycle: %llu\n", csr_rdcycle());
    printf("rdtime: %llu\n", csr_rdcycle());

    return 0;
}
