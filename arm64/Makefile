
#PROGS := wc args printenv argc
PROGS := argc inline crc32

CFLAGS := -g -Wall -march=armv8-a+crc
#CFLAGS := -O3 -Wall

all: ${PROGS}

#LDFLAGS := -s  #strip

wc: wc.o
args: args.o
argc: argc.o
printenv: printenv.o
inline:
crc32:

% : %.o
	ld ${LDFLAGS} -o $@ $^

%.o:%.S
	as -g -o $@ $^

clean:
	rm -f *.o ${PROGS}
