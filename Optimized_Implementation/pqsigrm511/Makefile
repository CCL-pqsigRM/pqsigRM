LDFLAGS =  -L/usr/local/lib
CFLAGS = -I/usr/local/include
LIBFLAGS = -lcrypto -lssl -lm
CC = gcc
CFILES := $(shell find . -name '*.c' | sed -e 's/\.c/\.o/')

OBJS = ${CFILES}

sign: ${OBJS}
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^  $(LIBFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

rng.o : rng.h
api.o : api.h

