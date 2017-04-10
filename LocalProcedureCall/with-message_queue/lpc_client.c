/*
 * Name : HwiYeon Cho
 * E-mail : hwyn.cho@gmail.com
 *
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:03
 */

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "lpc.h"
#include "lpc_client.h"

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

void Init(void)
{
	myKey = msgget((key_t)KEY, IPC_CREAT | PERMS);

	return;
}

FILE * myfopen(const char * path, const char * mode)
{
	char * fptr;
	FILE * fp;
	LpcDataMsg dataMsg;
	LpcResMsg resMsg;

	memset(&dataMsg, 0, sizeof(LpcDataMsg));
	memset(&resMsg, 0, sizeof(LpcResMsg));

	/* pack data message */
	dataMsg.type = 1;

	dataMsg.hdrPack.service = LPC_OPEN_FILE;
	dataMsg.hdrPack.numArg = 2;

	dataMsg.argPack1.argSize = strlen(path);
	strcpy(dataMsg.argPack1.argData, path);

	dataMsg.argPack2.argSize = strlen(mode);
	strcpy(dataMsg.argPack2.argData, mode);
	
	/* send data message to server */
	msgsnd(myKey, &dataMsg, sizeof(LpcDataMsg) - sizeof(long), 0);

	/* receive result message from server */
	msgrcv(myKey, &resMsg, sizeof(LpcResMsg) - sizeof(long), 2, 0);
	
	if (resMsg.result.errorno == 0)
	{
		fptr = (char *)malloc(resMsg.result.resultSize + 1);
		memset(fptr, 0x00, sizeof(fptr));
		strcpy(fptr, resMsg.result.resultData);
		sscanf(fptr, "%p", &fp);
		free(fptr);
	}
	
	else
		fp = NULL;
		
	return fp;
}

size_t myfread(void * ptr, size_t size, size_t nmemb, FILE * stream)
{
	char * fptr;
	FILE * fp;
	LpcDataMsg dataMsg;
	LpcResMsg resMsg;

	memset(&dataMsg, 0, sizeof(LpcDataMsg));
	memset(&resMsg, 0, sizeof(LpcResMsg));

	/* pack data message */
	dataMsg.type = 1;

	dataMsg.hdrPack.service = LPC_READ_FILE;
	dataMsg.hdrPack.numArg = 3;

	sprintf(dataMsg.argPack2.argData, "%lu", size);
	dataMsg.argPack2.argSize = strlen(dataMsg.argPack2.argData);

	sprintf(dataMsg.argPack3.argData, "%lu", nmemb);
	dataMsg.argPack3.argSize = strlen(dataMsg.argPack3.argData);

	sprintf(dataMsg.argPack4.argData, "%p", stream);
	dataMsg.argPack4.argSize = strlen(dataMsg.argPack4.argData);

	/* send data message to server */
	msgsnd(myKey, &dataMsg, sizeof(LpcDataMsg) - sizeof(long), 0);

	/* receive result message from server */
	msgrcv(myKey, &resMsg, sizeof(LpcResMsg) - sizeof(long), 2, 0);

	if (resMsg.result.errorno == 0)
		strcpy((char *)ptr, resMsg.result.resultData);
	
	else
		;
		
	return resMsg.result.resultSize;
}

size_t myfwrite(const void * ptr, size_t size, size_t nmemb, FILE * stream)
{
	LpcDataMsg dataMsg;
	LpcResMsg resMsg;

	memset(&dataMsg, 0, sizeof(LpcDataMsg));
	memset(&resMsg, 0, sizeof(LpcResMsg));
	
	/* pack data message */
	dataMsg.type = 1;

	dataMsg.hdrPack.service = LPC_WRITE_FILE;
	dataMsg.hdrPack.numArg = 4;

	dataMsg.argPack1.argSize = strlen((char *)ptr);
	strcpy(dataMsg.argPack1.argData, (char *)ptr);

	sprintf(dataMsg.argPack2.argData, "%lu", size);
	dataMsg.argPack2.argSize = strlen(dataMsg.argPack2.argData);

	sprintf(dataMsg.argPack3.argData, "%lu", nmemb);
	dataMsg.argPack3.argSize = strlen(dataMsg.argPack3.argData);

	sprintf(dataMsg.argPack4.argData, "%p", stream);
	dataMsg.argPack4.argSize = strlen(dataMsg.argPack4.argData);

	/* send data message to server */
	msgsnd(myKey, &dataMsg, sizeof(LpcDataMsg) - sizeof(long), 0);

	/* receive result message from server */
	msgrcv(myKey, &resMsg, sizeof(LpcResMsg) - sizeof(long), 2, 0);
	
	if (resMsg.result.errorno == 0)
		;
	
	else
		;
		
	return resMsg.result.resultSize;
}

int myfclose(FILE * fp)
{
	LpcDataMsg dataMsg;
	LpcResMsg resMsg;

	memset(&dataMsg, 0, sizeof(LpcDataMsg));
	memset(&resMsg, 0, sizeof(LpcResMsg));
	
	/* pack data message */
	dataMsg.type = 1;

	dataMsg.hdrPack.service = LPC_CLOSE_FILE;
	dataMsg.hdrPack.numArg = 1;

	sprintf(dataMsg.argPack1.argData, "%p", fp);
	dataMsg.argPack1.argSize = strlen(dataMsg.argPack1.argData);

	/* send data message to server */
	msgsnd(myKey, &dataMsg, sizeof(LpcDataMsg) - sizeof(long), 0);

	/* receive result message from server */
	msgrcv(myKey, &resMsg, sizeof(LpcResMsg) - sizeof(long), 2, 0);
	
	if (resMsg.result.errorno == 0)
		return 0;
	
	else
		return 1;
}

int myfflush(FILE * stream)
{
	LpcDataMsg dataMsg;
	LpcResMsg resMsg;

	memset(&dataMsg, 0, sizeof(LpcDataMsg));
	memset(&resMsg, 0, sizeof(LpcResMsg));
	
	/* pack data message */
	dataMsg.type = 1;

	dataMsg.hdrPack.service = LPC_FLUSH_FILE;
	dataMsg.hdrPack.numArg = 1;

	sprintf(dataMsg.argPack1.argData, "%p", stream);
	dataMsg.argPack1.argSize = strlen(dataMsg.argPack1.argData);

	/* send data message to server */
	msgsnd(myKey, &dataMsg, sizeof(LpcDataMsg) - sizeof(long), 0);

	/* receive result message from server */
	msgrcv(myKey, &resMsg, sizeof(LpcResMsg) - sizeof(long), 2, 0);

	if (resMsg.result.errorno == 0)
		return 0;
	
	else
		return 1;
}