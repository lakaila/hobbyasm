
all: wc args printenv

wc: wc.o
	ld -s -o $@ $^

args: args.o
	ld -s -o $@ $^

printenv: printenv.o
	ld -s -o $@ $^

%.o:%.S
	as -g -o $@ $^

clean:
	rm -f *.o wc args printenv
