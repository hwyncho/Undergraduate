/*
 * Name : HwiYeon Cho
 * E-mail : hwyn.cho@gmail.com
 *
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:08
 */
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "lpc.h"

#define PERMS 0777

void signal_handler(int signo);

void open_file(void);
void read_file(void);
void write_file(void);
void close_file(void);
void flush_file(void);

int main(void)
{
    printf(">>> Server's pid : %ld\n", (long)getpid());

    signal(SIGUSR1, signal_handler);
    signal(SIGINT, signal_handler);

    while (1)
    {
        printf(">>> Waiting...\n");
        pause();
    }

    return 0;
}

void signal_handler(int signo)
{
    int fd;
    LpcHdrPack hdrPack;

    if (signo == SIGUSR1)
    {
        printf(">>> I'm wake!\n");

        memset(&hdrPack, 0, sizeof(LpcHdrPack));

        /* unpack service data */
        fd = open("./hdr.dat", O_RDONLY);
        read(fd, &hdrPack, sizeof(LpcHdrPack));
        close(fd);

        switch (hdrPack.service)
        {
        case LPC_OPEN_FILE:
            open_file();
            break;

        case LPC_READ_FILE:
            read_file();
            break;

        case LPC_WRITE_FILE:
            write_file();
            break;

        case LPC_CLOSE_FILE:
            close_file();
            break;

        case LPC_FLUSH_FILE:
            flush_file();
            break;
        }

        /* send signal to client */
        usleep(1000);
        kill(hdrPack.pid, SIGUSR2);
    }

    else if (signo == SIGINT)
    {
        printf("\n");

        system("make clean");

        exit(1);
    }

    return;
}

void open_file(void)
{
    int fd;
    char * path, * mode;
    char fpstr[LPC_DATA_MAX];
    FILE * fp;
    LpcArgPack argPack1;
    LpcArgPack argPack2;
    LpcResult result;

    memset(&argPack1, 0, sizeof(LpcArgPack));
    memset(&argPack2, 0, sizeof(LpcArgPack));
    memset(&result, 0, sizeof(LpcResult));

    /* unpack argument data */
    fd = open("./arg.dat", O_RDONLY);
    read(fd, &argPack1, sizeof(LpcArgPack));
    read(fd, &argPack2, sizeof(LpcArgPack));
    close(fd);

    path = (char *)malloc(argPack1.argSize + 1);
    memset(path, 0x00, argPack1.argSize + 1);
    strcpy(path, argPack1.argData);

    mode = (char *)malloc(argPack2.argSize + 1);
    memset(mode, 0x00, argPack2.argSize + 1);
    strcpy(mode, argPack2.argData);

    fp = fopen(path, mode);

    /* pack result data */
    if (fp == NULL)
    {
        printf(">>> fopen : Fail!\n");

        result.errorno = 1;
        result.resultSize = 0;
    }

    else
    {
        printf(">>> fopen : Success!\n");

        result.errorno = 0;
        sprintf(result.resultData, "%p", fp);
        result.resultSize = strlen(result.resultData);
    }

    /* send result data */
    fd = open("./result.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
    write(fd, &result, sizeof(LpcResult));
    close(fd);

    free(path);
    free(mode);

    return;
}

void read_file(void)
{
    int fd;
    size_t size, nmemb, count;
    char buf[LPC_DATA_MAX];
    char * ptr;
    FILE * fp;
    LpcArgPack argPack2;
    LpcArgPack argPack3;
    LpcArgPack argPack4;
    LpcResult result;

    memset(&argPack2, 0, sizeof(LpcArgPack));
    memset(&argPack3, 0, sizeof(LpcArgPack));
    memset(&argPack3, 0, sizeof(LpcArgPack));
    memset(&result, 0, sizeof(LpcResult));

    /* unpack argument data */
    fd = open("./arg.dat", O_RDONLY);
    read(fd, &argPack2, sizeof(LpcArgPack));
    read(fd, &argPack3, sizeof(LpcArgPack));
    read(fd, &argPack4, sizeof(LpcArgPack));
    close(fd);

    sscanf(argPack2.argData, "%lu", &size);
    sscanf(argPack3.argData, "%lu", &nmemb);

    ptr = (char *)malloc(argPack4.argSize + 1);
    memset(ptr, 0x00, argPack4.argSize + 1);
    strcpy(ptr, argPack4.argData);
    sscanf(ptr, "%p", &fp);
    free(ptr);

    count = fread(buf, size, nmemb, fp);

    /* pack result data */
    if (count != nmemb)
    {
        printf(">>> fread : Fail!\n");

        result.errorno = 1;
        result.resultSize = 0;
    }

    else
    {
        printf(">>> fread : Success!\n");

        result.errorno = 0;
        result.resultSize = count;
        strcpy(result.resultData, buf);
    }

    /* send result data */
    fd = open("./result.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
    write(fd, &result, sizeof(LpcResult));
    close(fd);

    return;
}

void write_file(void)
{
    int fd;
    size_t size, nmemb, count;
    char buf[LPC_DATA_MAX];
    char * ptr;
    FILE * fp;
    LpcArgPack argPack1;
    LpcArgPack argPack2;
    LpcArgPack argPack3;
    LpcArgPack argPack4;
    LpcResult result;

    memset(&argPack1, 0, sizeof(LpcArgPack));
    memset(&argPack2, 0, sizeof(LpcArgPack));
    memset(&argPack3, 0, sizeof(LpcArgPack));
    memset(&argPack3, 0, sizeof(LpcArgPack));
    memset(&result, 0, sizeof(LpcResult));

    /* unpack argument data */
    fd = open("./arg.dat", O_RDONLY);
    read(fd, &argPack1, sizeof(LpcArgPack));
    read(fd, &argPack2, sizeof(LpcArgPack));
    read(fd, &argPack3, sizeof(LpcArgPack));
    read(fd, &argPack4, sizeof(LpcArgPack));
    close(fd);

    sscanf(argPack2.argData, "%lu", &size);
    sscanf(argPack3.argData, "%lu", &nmemb);

    ptr = (char *)malloc(argPack4.argSize + 1);
    memset(ptr, 0x00, argPack4.argSize + 1);
    strcpy(ptr, argPack4.argData);
    sscanf(ptr, "%p", &fp);
    free(ptr);

    count = fwrite(argPack1.argData, size, nmemb, fp);

    /* pack result data */
    if (count != nmemb)
    {
        printf(">>> fwrite : Fail!\n");

        result.errorno = 1;
        result.resultSize = 0;
    }

    else
    {
        printf(">>> fwrite : Success!\n");

        result.errorno = 0;
        result.resultSize = count;
    }

    /* send result data */
    fd = open("./result.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
    write(fd, &result, sizeof(LpcResult));
    close(fd);

    return;
}

void close_file(void)
{
    int fd, check;
    char * ptr;
    FILE * fp;
    LpcArgPack argPack;
    LpcResult result;

    memset(&argPack, 0, sizeof(LpcArgPack));
    memset(&result, 0, sizeof(LpcResult));

    /* unpack argument data */
    fd = open("./arg.dat", O_RDONLY);
    read(fd, &argPack, sizeof(LpcArgPack));
    close(fd);

    ptr = (char *)malloc(argPack.argSize + 1);
    memset(ptr, 0x00, argPack.argSize + 1);
    strcpy(ptr, argPack.argData);
    sscanf(ptr, "%p", &fp);
    free(ptr);

    check = fclose(fp);

    /* pack result data */
    if (check == 0)
    {
        printf(">>> fclose : Success!\n");

        result.errorno = 0;
    }

    else
    {
        printf(">>> fclose : Fail!\n");

        result.errorno = 1;
    }

    /* send result data */
    fd = open("./result.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
    write(fd, &result, sizeof(LpcResult));
    close(fd);

    return;
}

void flush_file(void)
{
    int fd, check;
    char * ptr;
    FILE * stream;
    LpcArgPack argPack;
    LpcResult result;

    memset(&argPack, 0, sizeof(LpcArgPack));
    memset(&result, 0, sizeof(LpcResult));

    /* unpack argument data */
    fd = open("./arg.dat", O_RDONLY);
    read(fd, &argPack, sizeof(LpcArgPack));
    close(fd);

    ptr = (char *)malloc(argPack.argSize + 1);
    memset(ptr, 0x00, argPack.argSize + 1);
    strcpy(ptr, argPack.argData);
    sscanf(ptr, "%p", &stream);
    free(ptr);

    check = fflush(stream);

    /* pack result data */
    if (check == 0)
    {
        printf(">>> fflush : Success!\n");

        result.errorno = 0;
    }

    else
    {
        printf(">>> fflush : Fail!\n");

        result.errorno = 1;
    }

    /* send result data */
    fd = open("./result.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
    write(fd, &result, sizeof(LpcResult));
    close(fd);

    return;
}
