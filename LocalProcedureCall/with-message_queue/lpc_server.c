/*
 * Name : HwiYeon Cho
 * E-mail : hwyn.cho@gmail.com
 *
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:04
 */
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include "lpc.h"

#define PERMS 0777
#define KEY 2014726096

/* declaration of message types */
typedef struct __lpcDataMsg
{
    long type;
    LpcHdrPack hdrPack;
    LpcArgPack argPack1;
    LpcArgPack argPack2;
    LpcArgPack argPack3;
    LpcArgPack argPack4;
} LpcDataMsg;

typedef struct __lpcResMsg
{
    long type;
    LpcResult result;
} LpcResMsg;

key_t myKey;

void signal_handler(int signo);

void open_file(LpcArgPack * pArgPack1, LpcArgPack * pArgPack2);
void read_file(LpcArgPack * pArgPack2, LpcArgPack * pArgPack3, LpcArgPack * pArgPack4);
void write_file(LpcArgPack * pArgPack1, LpcArgPack * pArgPack2, LpcArgPack * pArgPack3, LpcArgPack * pArgPack4);
void close_file(LpcArgPack * pArgPack1);
void flush_file(LpcArgPack * pArgPack1);

int main(void)
{
    LpcDataMsg dataMsg;

    myKey = msgget((key_t)KEY, IPC_CREAT | PERMS);
    
    signal(SIGINT, signal_handler);
    
    printf(">>> Hello!\n");

    while (1)
    {
        /* receive data message from client */
        memset(&dataMsg, 0, sizeof(LpcDataMsg));
        msgrcv(myKey, &dataMsg, sizeof(LpcDataMsg) - sizeof(long), 1, 0);

        printf(">>> I'm wake!\n");

        switch (dataMsg.hdrPack.service)
        {
        case LPC_OPEN_FILE:
            open_file(&dataMsg.argPack1, &dataMsg.argPack2);
            break;

        case LPC_READ_FILE:
            read_file(&dataMsg.argPack2, &dataMsg.argPack3, &dataMsg.argPack4);
            break;

        case LPC_WRITE_FILE:
            write_file(&dataMsg.argPack1, &dataMsg.argPack2, &dataMsg.argPack3, &dataMsg.argPack4);
            break;

        case LPC_CLOSE_FILE:
            close_file(&dataMsg.argPack1);
            break;

        case LPC_FLUSH_FILE:
            flush_file(&dataMsg.argPack1);
            break;
        }

        printf(">>> Waiting...\n");
    }

    return 0;
}

void signal_handler(int signo)
{
    if (signo == SIGINT)
    {
        printf("\n");
        
        msgctl(myKey, IPC_RMID, 0);
        system("make clean");

        exit(0);
    }

    return;
}

void open_file(LpcArgPack * pArgPack1, LpcArgPack * pArgPack2)
{
    char * path, * mode;
    FILE * fp;
    LpcResMsg resMsg;

    memset(&resMsg, 0, sizeof(LpcResMsg));

    path = (char *)malloc(pArgPack1->argSize + 1);
    memset(path, 0x00, sizeof(path));
    strcpy(path, pArgPack1->argData);

    mode = (char *)malloc(pArgPack2->argSize + 1);
    memset(mode, 0x00, sizeof(mode));
    strcpy(mode, pArgPack2->argData);

    fp = fopen(path, mode);

    if (fp == NULL)
    {
        printf(">>> fopen : Fail!\n");

        resMsg.type = 2;

        resMsg.result.errorno = 1;
    }

    else
    {
        printf(">>> fopen : Success!\n");

        resMsg.type = 2;

        resMsg.result.errorno = 0;
        sprintf(resMsg.result.resultData, "%p", fp);
        resMsg.result.resultSize = strlen(resMsg.result.resultData);
    }
    
    // send result message to client
    msgsnd(myKey, &resMsg, sizeof(LpcResMsg) - sizeof(long), 0);

    free(path);
    free(mode);

    return;
}

void read_file(LpcArgPack * pArgPack2, LpcArgPack * pArgPack3, LpcArgPack * pArgPack4)
{
    size_t size, nmemb, count;
    char buf[LPC_DATA_MAX];
    char * ptr;
    FILE * fp;
    LpcResMsg resMsg;

    memset(&resMsg, 0, sizeof(LpcResMsg));

    sscanf(pArgPack2->argData, "%lu", &size);
    sscanf(pArgPack3->argData, "%lu", &nmemb);

    ptr = (char *)malloc(pArgPack4->argSize + 1);
    memset(ptr, 0x00, sizeof(ptr));
    strcpy(ptr, pArgPack4->argData);
    sscanf(ptr, "%p", &fp);
    free(ptr);
    
    count = fread(buf, size, nmemb, fp);

    if (count != nmemb)
    {
        printf(">>> fread : Fail!\n");

        resMsg.type = 2;

        resMsg.result.errorno = 1;
        resMsg.result.resultSize = 0;
    }

    else
    {
        printf(">>> fread : Success!\n");

        resMsg.type = 2;

        resMsg.result.errorno = 0;
        resMsg.result.resultSize = count;
        strcpy(resMsg.result.resultData, buf);
    }

    // send result message to client
    msgsnd(myKey, &resMsg, sizeof(LpcResMsg) - sizeof(long), 0);

    return;
}

void write_file(LpcArgPack * pArgPack1, LpcArgPack * pArgPack2, LpcArgPack * pArgPack3, LpcArgPack * pArgPack4)
{
    size_t size, nmemb, count;
    char buf[LPC_DATA_MAX];
    char * ptr;
    FILE * fp;
    LpcResMsg resMsg;

    memset(&resMsg, 0, sizeof(LpcResMsg));

    sscanf(pArgPack2->argData, "%lu", &size);
    sscanf(pArgPack3->argData, "%lu", &nmemb);

    ptr = (char *)malloc(pArgPack4->argSize + 1);
    memset(ptr, 0x00, sizeof(ptr));
    strcpy(ptr, pArgPack4->argData);
    sscanf(ptr, "%p", &fp);
    free(ptr);

    count = fwrite(pArgPack1->argData, size, nmemb, fp);

    if (count != nmemb)
    {
        printf(">>> fwrite : Fail!\n");

        resMsg.type = 2;

        resMsg.result.errorno = 1;
        resMsg.result.resultSize = 0;
    }

    else
    {
        printf(">>> fwrite : Success!\n");

        resMsg.type = 2;

        resMsg.result.errorno = 0;
        resMsg.result.resultSize = count;
    }

    // send result message to client
    msgsnd(myKey, &resMsg, sizeof(LpcResMsg) - sizeof(long), 0);

    return;
}

void close_file(LpcArgPack * pArgPack1)
{
    int check;
    char * ptr;
    FILE * fp;
    LpcResMsg resMsg;

    memset(&resMsg, 0, sizeof(LpcResMsg));

    ptr = (char *)malloc(pArgPack1->argSize + 1);
    memset(ptr, 0x00, sizeof(ptr));
    strcpy(ptr, pArgPack1->argData);
    sscanf(ptr, "%p", &fp);
    free(ptr);

    check = fclose(fp);

    if (check == 0)
    {
        printf(">>> fclose : Success!\n");

        resMsg.type = 2;

        resMsg.result.errorno = 0;
    }

    else
    {
        printf(">>> fclose : Fail!\n");

        resMsg.type = 2;

        resMsg.result.errorno = 1;
    }

    // send result message to client
    msgsnd(myKey, &resMsg, sizeof(LpcResMsg) - sizeof(long), 0);

    return;
}

void flush_file(LpcArgPack * pArgPack1)
{
    int check;
    char * ptr;
    FILE * stream;
    LpcResMsg resMsg;

    memset(&resMsg, 0, sizeof(LpcResMsg));

    ptr = (char *)malloc(pArgPack1->argSize + 1);
    memset(ptr, 0x00, sizeof(ptr));
    strcpy(ptr, pArgPack1->argData);
    sscanf(ptr, "%p", &stream);
    free(ptr);

    check = fflush(stream);

    if (check == 0)
    {
        printf(">>> fflush : Success!\n");

        resMsg.type = 2;

        resMsg.result.errorno = 0;
    }
    
    else
    {
        printf(">>> fflush : Fail!\n");

        resMsg.type = 2;

        resMsg.result.errorno = 1;
    }

    // send result message to client
    msgsnd(myKey, &resMsg, sizeof(LpcResMsg) - sizeof(long), 0);

    return;
}
