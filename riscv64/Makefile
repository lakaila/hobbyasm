
PROGS := argc

CFLAGS := -g -Wall #-march=rv64gc
#CFLAGS := -O3 -Wall

# For cross-compilation (TODO: get a native dev env)
# ubuntu package gcc-riscv64-linux-gnu
LD := riscv64-linux-gnu-ld
AS := riscv64-linux-gnu-as
CC := riscv64-linux-gnu-gcc

all: ${PROGS}

#LDFLAGS := -s  #strip

argc: argc.o

% : %.o
	${LD} ${LDFLAGS} -o $@ $^

%.o:%.S
	${AS} -g -o $@ $^

clean:
	rm -f *.o ${PROGS}

# See syscall numbers:
# https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/include/uapi/asm-generic/unistd.h
