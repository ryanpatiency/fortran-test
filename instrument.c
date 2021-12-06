#define _GNU_SOURCE
#include <aio.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <linux/aio_abi.h>
#include <linux/limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "gfortran.h"

long CHUNK_SIZE = 4096;  // 1M is 1048576



void (*real__gfortran_st_open)(st_parameter_open *spopen);



__attribute__((constructor)) static void myinit()
{
    real__gfortran_st_open = dlsym(RTLD_NEXT, "_gfortran_st_open");
}


__attribute__((destructor)) static void myfinal() {}

void _gfortran_st_open(st_parameter_open *spopen)
{
    char *filename = malloc(spopen->file_len + 1);

    typedef GFC_INTEGER_4 GFC_IO_INT;
    strncpy(filename, spopen->file, spopen->file_len);
    filename[spopen->file_len] = '\0';
    printf("succeed in intercepting open, namelen is %d\n", spopen->file_len);
    printf("name is %s\n", filename);
    int(*x)[20] = spopen;
    for (int i = 0; i < 20; i++) {
        printf("i is %d, x is %d\n", i, (*x)[i]);
    }
    real__gfortran_st_open(spopen);
    exit(1);
}
