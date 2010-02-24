CFLAGS=-Wall -fPIC -g
LDFLAGS=-lm -Wl,-rpath .

all: libcombstruct.a libcombstruct.so libcombstruct_compile.so \
	   libcombstruct_blas.so libcombstruct_compile_blas.so

%_compile.o: %.c
	$(CC) -c $(CFLAGS) -DCOMPILE -o $@ $<

common.o: common.c common.h

diff.o: diff.c diff.h equation.h combstruct.h system.h common.h

equation.o: equation.c equation.h combstruct.h

eval.o: eval.c eval.h combstruct.h common.h equation.h

iter.o: iter.c combstruct.h eval.h system.h jacob.h diff.h equation.h

iter_blas.o: iter.c combstruct.h eval.h system.h jacob.h diff.h equation.h
	$(CC) -c $(CFLAGS) -DBLAS -o $@ $<

iter_compile.o: iter.c combstruct.h eval.h system.h jacob.h diff.h \
                equation.h compile.h

iter_compile_blas.o: iter.c combstruct.h eval.h system.h jacob.h diff.h \
                     equation.h compile.h
	$(CC) -c $(CFLAGS) -DCOMPILE -DBLAS -o $@ $<

jacob.o: jacob.c jacob.h combstruct.h equation.h system.h diff.h

print.o: print.c print.h combstruct.h jacob.h common.h equation.h \
         system.h

system.o: system.c system.h combstruct.h equation.h

system_compile.o: system.c system.h combstruct.h equation.h compile.h

compile.o: compile.c compile.h
	$(CC) -c $(CFLAGS) -DCOMPILE -o $@ $<

libcombstruct.a: system.o iter.o jacob.o diff.o eval.o print.o \
                 equation.o common.o
	ar rcs $@ $^

libcombstruct.so: system.o iter.o jacob.o diff.o eval.o print.o \
                  equation.o common.o
	$(CC) $(LDFLAGS) -shared -o $@ $^

libcombstruct_compile.so: compile.o system_compile.o iter_compile.o \
                          jacob.o diff.o print.o equation.o common.o
	$(CC) $(LDFLAGS) -ldl -shared -o $@ $^

libcombstruct_blas.so: system.o iter_blas.o jacob.o diff.o eval.o print.o \
	                     equation.o common.o
	$(CC) $(LDFLAGS) -lblas -shared -o $@ $^

libcombstruct_compile_blas.so: compile.o system_compile.o iter_compile_blas.o \
                               jacob.o diff.o print.o equation.o common.o
	$(CC) $(LDFLAGS) -lblas -ldl -shared -o $@ $^

clean:
	rm -f *.o *.so
