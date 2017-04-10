/*
 * Name : HwiYeon Cho
 * E-mail : hwyn.cho@gmail.com
 *
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:07
 */

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "lpc.h"
#include "lpc_client.h"

#define PERMS 0777

void signal_handler(int signo)
{
	if (signo == SIGUSR2)
		;
}

void Init(void)
{
	// install the signal handler
	signal(SIGUSR2, signal_handler);
	
	return;
}

FILE * myfopen(const char * path, const char * mode)
{
	int fd;
	char * fptr;
	FILE * fp;
	LpcArgPack argPack1;
	LpcArgPack argPack2;
	LpcHdrPack hdrPack;
	LpcResult result;
	
	memset(&argPack1, 0, sizeof(LpcArgPack));
	memset(&argPack2, 0, sizeof(LpcArgPack));
	memset(&hdrPack, 0, sizeof(LpcHdrPack));
	memset(&result, 0, sizeof(LpcResult));
	
	/* pack argument data */
	argPack1.argSize = strlen(path);
	strcpy(argPack1.argData, path);
	
	argPack2.argSize = strlen(mode);
	strcpy(argPack2.argData, mode);
	
	/* send argument data */
	fd = open("./arg.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	write(fd, &argPack1, sizeof(LpcArgPack));
	write(fd, &argPack2, sizeof(LpcArgPack));
	close(fd);
	
	/* pack service data */
	hdrPack.pid = getpid();
	hdrPack.service = LPC_OPEN_FILE;
	hdrPack.numArg = 2;
	
	/* send service data */
	fd = open("./hdr.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	write(fd, &hdrPack, sizeof(LpcHdrPack));
	close(fd);
	
	kill(pid, SIGUSR1);		// send signal to server
	
	pause();				// wait server's response
	
	/* unpack result data */
	fd = open("./result.dat", O_RDONLY);
	read(fd, &result, sizeof(LpcResult));
	close(fd);
	
	if (result.errorno == 0)
	{
		fptr = (char *)malloc(result.resultSize + 1);
		memset(fptr, 0x00, result.resultSize + 1);
		strcpy(fptr, result.resultData);
		sscanf(fptr, "%p", &fp);
		free(fptr);
	}
	
	else
		fp = NULL;
	
	return fp;
}

size_t myfread(void * ptr, size_t size, size_t nmemb, FILE * stream)
{
	int fd;
	LpcArgPack argPack2;
	LpcArgPack argPack3;
	LpcArgPack argPack4;
	LpcHdrPack hdrPack;
	LpcResult result;
	
	memset(&argPack2, 0, sizeof(LpcArgPack));
	memset(&argPack3, 0, sizeof(LpcArgPack));
	memset(&argPack4, 0, sizeof(LpcArgPack));
	memset(&hdrPack, 0, sizeof(LpcHdrPack));
	memset(&result, 0, sizeof(LpcResult));
	
	/* pack argument data */
	sprintf(argPack2.argData, "%lu", size);
	argPack2.argSize = strlen(argPack2.argData);
	
	sprintf(argPack3.argData, "%lu", nmemb);
	argPack3.argSize = strlen(argPack3.argData);
	
	sprintf(argPack4.argData, "%p", stream);
	argPack4.argSize = strlen(argPack4.argData);
	
	/* send argument data */
	fd = open("./arg.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	write(fd, &argPack2, sizeof(LpcArgPack));
	write(fd, &argPack3, sizeof(LpcArgPack));
	write(fd, &argPack4, sizeof(LpcArgPack));
	close(fd);
	
	/* pack service data */
	hdrPack.pid = getpid();
	hdrPack.service = LPC_READ_FILE;
	hdrPack.numArg = 3;
	
	/* send service data */
	fd = open("./hdr.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	write(fd, &hdrPack, sizeof(LpcHdrPack));
	close(fd);
	
	kill(pid, SIGUSR1);		// send signal to server
	
	pause();				// wait server's response
	
	/* unpack result data */
	fd = open("./result.dat", O_RDONLY);
	read(fd, &result, sizeof(LpcResult));
	close(fd);

	if (result.errorno == 0)
		strcpy((char *)ptr, result.resultData);
	
	else
		;
	
	return result.resultSize;
}

size_t myfwrite(const void * ptr, size_t size, size_t nmemb, FILE * stream)
{
	int fd;
	LpcArgPack argPack1;
	LpcArgPack argPack2;
	LpcArgPack argPack3;
	LpcArgPack argPack4;
	LpcHdrPack hdrPack;
	LpcResult result;
	
	memset(&argPack1, 0, sizeof(LpcArgPack));
	memset(&argPack2, 0, sizeof(LpcArgPack));
	memset(&argPack3, 0, sizeof(LpcArgPack));
	memset(&argPack3, 0, sizeof(LpcArgPack));
	memset(&result, 0, sizeof(LpcResult));
	
	/* pack argument data */
	argPack1.argSize = strlen((char *)ptr);
	strcpy(argPack1.argData, (char *)ptr);
		
	sprintf(argPack2.argData, "%lu", size);
	argPack2.argSize = strlen(argPack2.argData);
	
	sprintf(argPack3.argData, "%lu", nmemb);
	argPack3.argSize = strlen(argPack3.argData);
	
	sprintf(argPack4.argData, "%p", stream);
	argPack4.argSize = strlen(argPack4.argData);
	
	/* send argument data */
	fd = open("./arg.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	write(fd, &argPack1, sizeof(LpcArgPack));
	write(fd, &argPack2, sizeof(LpcArgPack));
	write(fd, &argPack3, sizeof(LpcArgPack));
	write(fd, &argPack4, sizeof(LpcArgPack));
	close(fd);

	/* pack service data */
	hdrPack.pid = getpid();
	hdrPack.service = LPC_WRITE_FILE;
	hdrPack.numArg = 4;
	
	/* send service data */
	fd = open("./hdr.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	write(fd, &hdrPack, sizeof(LpcHdrPack));
	close(fd);
	
	kill(pid, SIGUSR1);		// send signal to server
	
	pause();				// wait server's response
	
	/* unpack result data */
	fd = open("./result.dat", O_RDONLY);
	read(fd, &result, sizeof(LpcResult));
	close(fd);

	if (result.errorno == 0)
		;
	
	else
		;
		
	return result.resultSize;
}

int myfclose(FILE * fp)
{
	int fd;
	LpcArgPack argPack;
	LpcHdrPack hdrPack;
	LpcResult result;
	
	memset(&argPack, 0, sizeof(LpcArgPack));
	memset(&hdrPack, 0, sizeof(LpcHdrPack));
	memset(&result, 0, sizeof(LpcResult));
	
	/* pack argument data */
	sprintf(argPack.argData, "%p", fp);
	argPack.argSize = strlen(argPack.argData);
	
	/* send argument data */
	fd = open("./arg.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	write(fd, &argPack, sizeof(LpcArgPack));
	close(fd);
	
	/* pack service data */
	hdrPack.pid = getpid();
	hdrPack.service = LPC_CLOSE_FILE;
	hdrPack.numArg = 1;
	
	/* send service data */
	fd = open("./hdr.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	write(fd, &hdrPack, sizeof(LpcHdrPack));
	close(fd);
	
	kill(pid, SIGUSR1);		// send signal to server
	
	pause();				// wait server's response
	
	/* unpack result data */
	fd = open("./result.dat", O_RDONLY);
	read(fd, &result, sizeof(LpcResult));
	close(fd);
	
	if (result.errorno == 0)
		return 0;
	
	else
		return 1;
}

int myfflush(FILE * stream)
{
	int fd;
	LpcArgPack argPack;
	LpcHdrPack hdrPack;
	LpcResult result;
	
	memset(&argPack, 0, sizeof(LpcArgPack));
	memset(&hdrPack, 0, sizeof(LpcHdrPack));
	memset(&result, 0, sizeof(LpcResult));
	
	/* pack argument data */
	sprintf(argPack.argData, "%p", stream);
	argPack.argSize = strlen(argPack.argData);
	
	/* send argument data */
	fd = open("./arg.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	write(fd, &argPack, sizeof(LpcArgPack));
	close(fd);

	/* pack service data */
	hdrPack.pid = getpid();
	hdrPack.service = LPC_FLUSH_FILE;
	hdrPack.numArg = 1;
	
	/* send service data */
	fd = open("./hdr.dat", O_WRONLY | O_CREAT | O_TRUNC, PERMS);
	write(fd, &hdrPack, sizeof(LpcHdrPack));
	close(fd);
	
	kill(pid, SIGUSR1);		// send signal to server
	
	pause();				// wait server's response
	
	/* unpack result data */
	fd = open("./result.dat", O_RDONLY);
	read(fd, &result, sizeof(LpcResult));
	close(fd);
	
	if (result.errorno == 0)
		return 0;
	
	else
		return 1;
}