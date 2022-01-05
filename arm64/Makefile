
#PROGS := wc args printenv argc
PROGS := argc inline

CFLAGS := -g -Wall -march=armv8-a+crc
#CFLAGS := -O3 -Wall

all: ${PROGS}

#LDFLAGS := -s  #strip

wc: wc.o
args: args.o
argc: argc.o
printenv: printenv.o
inline:

% : %.o
	ld ${LDFLAGS} -o $@ $^

%.o:%.S
	as -g -o $@ $^

clean:
	rm -f *.o ${PROGS}
