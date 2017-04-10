/*
 * Name : HwiYeon Cho
 * E-mail : hwyn.cho@gmail.com
 *
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:03
 */

#ifndef __LPC_H__
#define __LPC_H__

#include <sys/types.h>
#include <unistd.h>

/* definition of lpc parameters */
#define LPC_DATA_MAX 1024
#define LPC_ARG_MAX 10

/* declaration of lpc types */
typedef enum __lpcService
{
	LPC_OPEN_FILE = 0,
	LPC_READ_FILE,
	LPC_WRITE_FILE,
	LPC_CLOSE_FILE,
	LPC_FLUSH_FILE
} LpcService;

typedef struct __lpcArgMsg
{
	pid_t pid;	// message type
	int argSize;
	char argData[LPC_DATA_MAX];
} LpcArgPack;

typedef struct __lpcHdrPack
{
	pid_t pid;	// message type
	LpcService service;
	int numArg;
} LpcHdrPack;

typedef struct __lpcResult
{
	int errorno;
	int resultSize;
	char resultData[LPC_DATA_MAX];
} LpcResult;

#endif /* !__LPC_H__ */