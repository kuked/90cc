CFLAGS=-std=c11 -g -static

90cc: 90cc.c

test: 90cc
		./test.sh

clean:
		rm -f 90cc *.o *~ tmp*

.PHONY: test clean
