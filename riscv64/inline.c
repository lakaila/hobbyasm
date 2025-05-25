
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

uint64_t csr_rdinstret(void)
{
    uint64_t val = 0;
    asm("rdinstret %[VAL];"
        : [VAL] "=r" (val) : : );
    return val;
}

int main()
{
    puts("inline hello!");

    uint64_t cycle1 = csr_rdcycle();
    uint64_t time1 = csr_rdtime();
    printf("rdcycle1: %llu\n", cycle1);
    printf("rdtime1: %llu\n", time1);
    uint64_t cycle2 = csr_rdcycle();
    uint64_t time2 = csr_rdtime();
    printf("rdcycle2: %llu (diff: %llu)\n", cycle2, cycle2-cycle1);
    printf("rdtime2: %llu (diff: %llu)\n", time2, time2-time1);
    uint64_t a = csr_rdinstret();
    uint64_t b = csr_rdinstret();
    printf("rdinstret: %llu\n", a);
    printf("rdinstret: %llu (diff: %llu)\n", b, b-a);

    return 0;
}
