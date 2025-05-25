
// Work in progress, not tested in HW yet

    
// as the RISC-V manual suggests, check for low part wrap-around

uint64_t csr_rdcycle(void)
{
    uint32_t hi = 0;
    uint32_t lo = 0;
    asm volatile (
	"1: rdcycleh t1;"
	"rdcycle %[LO];"
	"rdcycleh %[HI];"
	"bne t1, %[HI], 1b;"
        : [LO] "=r" (lo),
	  [HI] "=r" (hi)
	  : : "t1");
    uint64_t val = hi;
    val <<= 32;
    val |= lo;
    return val;
}

uint64_t csr_rdtime(void)
{
    uint32_t hi = 0;
    uint32_t lo = 0;
    asm volatile (
	"1: rdtimeh t1;"
	"rdtime %[LO];"
	"rdtimeh %[HI];"
	"bne t1, %[HI], 1b;"
        : [LO] "=r" (lo),
	  [HI] "=r" (hi)
	  : : "t1");
    uint64_t val = hi;
    val <<= 32;
    val |= lo;
    return val;
}

uint64_t csr_rdinstret(void)
{
    uint32_t hi = 0;
    uint32_t lo = 0;
    asm volatile (
	"1: rdinstreth t1;"
	"rdinstret %[LO];"
	"rdinstreth %[HI];"
	"bne t1, %[HI], 1b;"
        : [LO] "=r" (lo),
	  [HI] "=r" (hi)
	  : : "t1");
    uint64_t val = hi;
    val <<= 32;
    val |= lo;
    return val;
}

