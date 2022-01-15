
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static char buf[1024 * 32];

int main(int argc, char *argv[])
{
    int fd = 0; // stdin if filename not given
    uint32_t crc = 0xFFFFFFFF;

    if (argc > 2)
    {
        puts("too many argments");
        return 1;
    }
    if (argc == 2)
    {
        fd = open(argv[1], O_RDONLY);
        if (fd < 0)
        {
            perror("open");
            return 1;
        }
    }

    size_t ret = 0;
    do
    {
        ret = read(fd, buf, sizeof(buf));
        if (ret > 0)
        {
            size_t n = ret;
            const char *ptr = buf;
            asm("cmp %[LEN], xzr;"
                "b 1f;"
                "2: subs %[LEN], %[LEN], #1;"
                "1: b.eq 3f;"
                "ldrb w1, [%[P]], #1;"
                "crc32b %w[CRC], %w[CRC], w1;"
                "b 2b;"
                "3:"
                : [CRC] "+r"(crc), [P] "+r"(ptr), [LEN] "+r"(n)
                : "m"(buf)
                : "w1");
        }
    } while (ret > 0);

    crc ^= 0xFFFFFFFF;
    printf("%08X\n", crc);

    return 0;
}
