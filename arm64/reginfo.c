
#include <stdio.h>
#include <stdint.h>

// RNDR, bits [63:60]
// TLB, bits [59:56]
// TS, bits [55:52]
// FHM, bits [51:48]
// DP, bits [47:44]
// SM4, bits [43:40
// SM3, bits [39:36]
// SHA3, bits [35:32]
// RDM, bits [31:28]
// reserved Bits [27:24]
// Atomic, bits [23:20]
// CRC32, bits [19:16]
// SHA2, bits [15:12]
// SHA1, bits [11:8]
// AES, bits [7:4]
// Bits [3:0] reserved
#define CRC32_BITFIELD (0xF<<16)
#define CRC32_SUPPORTED (0x1<<16)

int main()
{
    puts("System Register information");

    uint64_t reg = 0;

    asm ("mrs %[REG], ID_AA64ISAR0_EL1;"
	 :[REG] "+r" (reg)
	 : 
	 :);
    printf("ID_AA64ISAR0_EL1: 0x%lX\n", reg);

    switch (reg & CRC32_BITFIELD) {
	case 0:
	    puts("CRC32 instructions not available.");
	    break;
	case CRC32_SUPPORTED:
	    puts("CRC32 instructions available.");
	    break;
	default:
	    puts("Unknown CRC32 bitfield value");
    }
    
    
    return 0;
}
