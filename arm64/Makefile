
#PROGS := wc args printenv argc
PROGS := argc inline crc32 crc32opt callasm wc reginfo crccrypt crcdecrypt

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
crc32opt:
callasm: functions.h functions.o
reginfo:
crccrypt:

crcdecrypt: | crccrypt
	ln -s $| $@

% : %.o
	ld ${LDFLAGS} -o $@ $^

%.o:%.S
	as -g -o $@ $^

clean:
	rm -f *.o ${PROGS}
