#define _GNU_SOURCE
#include "gfortran.h"
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
struct FileInfo {
    char* path;
    char* chunktype; // bytemap in fact
    char** buffer;
    long filesize;
    int nchunks;
    int lock_nchunks;
    int in_drive;
};
#define OPENED_MAX 20
struct FileInfo fi[OPENED_MAX];

long CHUNK_SIZE = 4096; // 1M is 1048576

int (*real_open)(const char* path, int flags, ...);
int (*real_open64)(const char* path, int flags, ...);
ssize_t (*real_read)(int fd, void* buf, size_t noct);
ssize_t (*real_write)(int fd, const void* buf, size_t count);
int (*real_creat64)(const char* pathname, mode_t mode);
DIR* (*real_opendir)(const char* path);
int (*real_closedir)(DIR* dirp);
int (*real_close)(int fd);
int (*real_unlink)(const char* path);
off_t (*real_lseek64)(int fd, off_t offset, int whence);
int (*real___lxstat64)(int ver, const char* path, struct stat64* stat_buf);
int (*real___xstat64)(int ver, const char* path, struct stat64* stat_buf);
int (*real_fsync)(int fd);

// #@#@#@#@#@#@#@#@#@@##@ below are all new thing

#define MAX_UNIT 100
#define UNIT_NO_FD -1
int unit_to_fd[MAX_UNIT];

void (*real__gfortran_st_open)(st_parameter_open* stpopn);
void (*real__gfortran_transfer_real_write)(st_parameter_dt* stpdt, void* buff, int size);
void (*real__gfortran_transfer_real)(st_parameter_dt* stpdt, void* buff, int size);

__attribute__((constructor)) static void myinit()
{
    real_creat64 = dlsym(RTLD_NEXT, "creat64");
    real_open = dlsym(RTLD_NEXT, "open");
    real_open64 = dlsym(RTLD_NEXT, "open64");
    real_opendir = dlsym(RTLD_NEXT, "opendir");
    real_closedir = dlsym(RTLD_NEXT, "closedir");
    real_close = dlsym(RTLD_NEXT, "close");
    real_unlink = dlsym(RTLD_NEXT, "unlink");
    real_read = dlsym(RTLD_NEXT, "read");
    real_write = dlsym(RTLD_NEXT, "write");
    real_lseek64 = dlsym(RTLD_NEXT, "lseek64");
    real___lxstat64 = dlsym(RTLD_NEXT, "__lxstat64");
    real___xstat64 = dlsym(RTLD_NEXT, "__xstat64");
    real_fsync = dlsym(RTLD_NEXT, "fsync");
    // #%#%#%#%#%#
    real__gfortran_st_open = dlsym(RTLD_NEXT, "_gfortran_st_open");
    real__gfortran_transfer_real_write = dlsym(RTLD_NEXT, "_gfortran_transfer_real_write");
    real__gfortran_transfer_real = dlsym(RTLD_NEXT, "_gfortran_transfer_real");
    for (int i = 0; i < MAX_UNIT; i++) {
        unit_to_fd[i] = UNIT_NO_FD;
    }
}

__attribute__((destructor)) static void myfinal() { }

void _gfortran_st_open(st_parameter_open* stpopn)
{
    // char* filename = malloc(stpopn->file_len + 1);

    typedef GFC_INTEGER_4 GFC_IO_INT;
    // strncpy(filename, stpopn->file, stpopn->file_len);
    // filename[stpopn->file_len] = '\0';
    char file[100];
    char status[10];
    char position[10];
    strncpy(file, stpopn->file, stpopn->file_len);
    strncpy(status, stpopn->status, stpopn->status_len);
    strncpy(position, stpopn->position, stpopn->position_len);
    file[stpopn->file_len] = '\0';
    status[stpopn->status_len] = '\0';
    position[stpopn->position_len] = '\0';

    printf("%s\n", stpopn->file);
    printf("file name is %s, len is %d\n", file, stpopn->file_len);
    printf("status name is %s, len is %d\n", status, stpopn->status_len);
    printf("position name is %s, len is %d\n", position, stpopn->position_len);

    printf("unit: %d, fd: %d\n", stpopn->common.unit,
        unit_to_fd[stpopn->common.unit]);

    exit(-1);
    // int ret = real_open64(filename, flags, mode);
}
void _gfortran_transfer_real_write(st_parameter_dt* stpdt, void* buff, int size)
{
    int unit = stpdt->common.unit;
    int fd = unit_to_fd[unit];
    real__gfortran_transfer_real_write(stpdt, buff, size);
}
void _gfortran_transfer_real(st_parameter_dt* stpdt, void* buff, int size)
{
    real__gfortran_transfer_real(stpdt, buff, size);
}