all: instrument.so  

instrument.so: instrument.c gfortran.h
	gcc -Wall -shared -fPIC -o instrument.so instrument.c \
	-ldl -lrt -lpthread 


test.out: test.f08
	gfortran test.f08 -o test.out

.PHONY: clean  test ltrace test2 dump

test: instrument.so test.out
	LD_PRELOAD=./instrument.so ./test.out

ltrace:
	ltrace -f -c ./a.out

dump:
	gfortran -fdump-tree-original test.f08 -o test.out