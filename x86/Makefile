
PROGS := argc

all: ${PROGS}

#wc: wc.o
#args: args.o
argc: argc.o
#printenv: printenv.o

% : %.o
	ld -m elf_i386 -s -o $@ $^

# Allow plain intel-style syntax - just a personal preference
%.o:%.S
	as -mmnemonic=intel -msyntax=intel -mnaked-reg --32 -g -o $@ $^

clean:
	rm -f *.o ${PROGS}
