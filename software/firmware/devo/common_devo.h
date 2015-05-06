#ifndef _COMMON_DEVO_H_
#define _COMMON_DEVO_H_

#ifndef FILE_SIZE
#include "ff.h"
#define FILE_SIZE sizeof(FATFS)
#endif

#define fat_mount      f_mount
#define fat_open       f_open
#define fat_read       f_read
#define fat_lseek      f_lseek
#define fat_close      f_close
#define fat_opendir    f_opendir
#define fat_readdir    f_readdir
#define fat_write      f_write

#include "ports.h"

#endif //_COMMON_DEVO_H_
