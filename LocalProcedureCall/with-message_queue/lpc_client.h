/*
 * Name : HwiYeon Cho
 * E-mail : hwyn.cho@gmail.com
 *
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:04
 */
#ifndef __LPC_CLIENT_H__
#define __LPC_CLIENT_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void Init(void);

FILE * myfopen(const char * path, const char * mode);
size_t myfread(void * ptr, size_t size, size_t nmemb, FILE * stream);
size_t myfwrite(const void * ptr, size_t size, size_t nmemb, FILE * stream);
int myfclose(FILE * fp);
int myfflush(FILE * stream);

extern int pid;

#endif /* !__LPC_CLIENT_H__ */
