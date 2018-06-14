CFLAGS=-Wall -g
LDFLAGS=-L../src -lm -Wl,-rpath ../src
CC=gcc

all: test

%: %.c ../src/boltzoc.h
	$(CC) $(CFLAGS) $(LDFLAGS) -lcombstruct -o $@ $<

%_compile_blas: %.c ../src/boltzoc.h
	$(CC) $(CFLAGS) $(LDFLAGS) -lcombstruct_compile_blas -o $@ $<

%_blas: %.c ../src/boltzoc.h
	$(CC) $(CFLAGS) $(LDFLAGS) -lcombstruct_blas -o $@ $<

%_compile: %.c ../src/boltzoc.h
	$(CC) $(CFLAGS) $(LDFLAGS) -lcombstruct_compile -o $@ $<

clean:
	rm -f test *.o
