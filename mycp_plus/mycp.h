#ifndef __MYCP_H
#define __MYCP_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#define BUFSIZE 4096

int mycp(const char *src, const char *dest);

int file_cp(const char *src, const char *dest);

int dir_copy_recursive(const char *src_dir, const char *dest_dir);


#endif
