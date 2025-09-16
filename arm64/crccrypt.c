
#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PREFIX_LEN 5

static uint32_t crc32(uint32_t crc, uint8_t byte)
{
    __asm__ volatile(
        "crc32b %w[CRC], %w[CRC], %w[B];"
        : [CRC] "+r"(crc)
        : [B] "r"(byte));
    return crc;
}

static uint32_t crc32_final(uint32_t crc)
{
    return crc ^ 0xFFFFFFFF;
}

int test_crc()
{
    // for testing possible another CRC implementation

    uint32_t crc = 0xFFFFFFFF;
    crc = crc32(crc, 'T');
    printf("%8X\n", crc32_final(crc));
    assert(crc32_final(crc) == 0xBE047A60);
    crc = crc32(crc, 'e');
    printf("%8X\n", crc32_final(crc));
    assert(crc32_final(crc) == 0xA2D61F78);
    crc = crc32(crc, 's');
    printf("%8X\n", crc32_final(crc));
    assert(crc32_final(crc) == 0x4572E01A);
    crc = crc32(crc, 't');
    printf("%8X\n", crc32_final(crc));
    assert(crc32_final(crc) == 0x784DD132);
    crc = crc32(crc, '\n');
    printf("%8X\n", crc32_final(crc));
    assert(crc32_final(crc) == 0xFA781AC2);

    return 0;
}

int main(int argc, char* argv[])
{
    puts("crccrypt");

    int ret = 1;
    uint32_t crc = 0xFFFFFFFF;

    if (argc != 4) {
        puts("Invalid number of arguments");
        goto err1;
    }
    int decrypt = strstr(argv[0], "decrypt") ? 1 : 0;
    const char* keyfile = argv[1];
    const char* infile = argv[2];
    const char* outfile = argv[3];

    int keyfd = open(keyfile, O_RDONLY);
    if (keyfd < 0) {
        printf("Unable to open file '%s': %m\n", keyfile);
        goto err1;
    }

    struct stat st;
    if (fstat(keyfd, &st)) {
        perror("stat");
        goto err2;
    }

    uint8_t* keybuf = malloc(st.st_size);
    if (!keybuf) {
        perror("malloc");
        goto err2;
    }
    ssize_t r = read(keyfd, keybuf, st.st_size);
    if (r != st.st_size) {
        if (r < 0)
            perror("read");
        else
            puts("read incomplete");
        goto err3;
    }

    int infd = open(infile, O_RDONLY);
    if (infd < 0) {
        printf("Unable to open file '%s': %m\n", infile);
        goto err3;
    }

    int outfd = creat(outfile, 0600);
    if (outfd < 0) {
        printf("Unable to create file '%s': %m\n", outfile);
        goto err4;
    }

    int prefixDone = 0;
    do {
        unsigned char buf[4 * 1024];
        if (!decrypt && !prefixDone) {
            int randfd = open("/dev/random", O_RDONLY);
            if (randfd < 0) {
                perror("open(/dev/random)");
                goto err7;
            }
            r = read(randfd, buf, PREFIX_LEN);
            close(randfd);
            if (r < 0) {
                perror("read");
                goto err7;
            }
            if (r != PREFIX_LEN) {
                puts("read not PREFIX_LEN");
                goto err7;
            }
            prefixDone = 1;

        } else {
            r = read(infd, buf, sizeof(buf));
            if (r < 0) {
                perror("read");
                goto err7;
            }
        }

        for (int i = 0; i < r; i++) {
            unsigned char tmp = buf[i] ^ crc32_final(crc);
            unsigned char* hashbyte;
            if (decrypt) {
                hashbyte = &tmp;
            } else {
                hashbyte = &buf[i];
            }

            crc = crc32(crc, *hashbyte);

            buf[i] = tmp;
        }

        const unsigned char* writePtr = buf;
        if (decrypt && !prefixDone) {
            writePtr += PREFIX_LEN;
            r -= PREFIX_LEN;
            assert(r > 0);
            prefixDone = 1;
        }

        r = write(outfd, writePtr, r);
        if (r < 0) {
            perror("write");
            goto err7;
        }

    } while (r > 0);

    ret = 0; // now OK
err7:
    if (ret)
        unlink(outfile);
    close(outfd);
err4:
    close(infd);
err3:
    free(keybuf);
err2:
    close(keyfd);
err1:
    return ret;
}
