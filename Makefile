all: instrument.so  

instrument.so: instrument.c gfortran.h
	gcc -Wall -shared -fPIC -o instrument.so instrument.c \
	-ldl -lrt -lpthread 



.PHONY: clean  test ltrace test2 

test: instrument.so
	LD_PRELOAD=./instrument.so ./a.out

ltrace:
	ltrace -f -c ./a.out
	