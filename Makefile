
PROGS := wc args printenv argc

all: ${PROGS}

wc: wc.o
args: args.o
argc: argc.o
printenv: printenv.o

% : %.o
	ld -s -o $@ $^

%.o:%.S
	as -g -o $@ $^

clean:
	rm -f *.o ${PROGS}
