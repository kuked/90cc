CFLAGS=-std=c11 -g -static -Wall -Wno-switch

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

90cc: $(OBJS)
		$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): 90cc.h

test: 90cc
		./test.sh

clean:
		rm -f 90cc *.o *~ tmp*

.PHONY: test clean
