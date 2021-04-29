
all: wc args

wc: wc.o
	ld -s -o $@ $^

args: args.o
	ld -s -o $@ $^

%.o:%.S
	as -g -o $@ $^

clean:
	rm -f *.o wc args
