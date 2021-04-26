
all: wc

wc: wc.o
	ld -s -o $@ $^

%.o:%.S
	as -g -o $@ $^

clean:
	rm -f *.o wc
