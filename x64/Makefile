
PROGS := argc inline

CFLAGS := -masm=intel

all: ${PROGS}

#wc: wc.o
#args: args.o
argc: argc.o
inline:
#printenv: printenv.o

% : %.o
	ld -m elf_x86_64 -s -o $@ $^

# Allow plain intel-style syntax - just a personal preference
%.o:%.S
	as -mmnemonic=intel -msyntax=intel -mnaked-reg --64 -g -o $@ $^

clean:
	rm -f *.o ${PROGS}
