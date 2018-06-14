
boltzoc: src/*.c libcombstruct2json.a combstruct2json.h
	gcc -o boltzoc -Wno-unused-value \
		-L. -I/opt/local/include -lcombstruct2json \
		src/oracle.c \
		src/symbtab.c src/system.c src/iter.c src/jacob.c src/diff.c \
		src/eval.c src/print.c src/equation.c src/common.c src/parse.c

all:
	make -C src
	make -C test

clean:
	make -C src clean
	make -C test clean
