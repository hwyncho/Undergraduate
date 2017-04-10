#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>  
#include <time.h>
#include "lpc_client.h"
#include "lpc.h"

#define TC__FILE_NAME "test_case.txt"
#define BLOCK_SIZE 10
#define BUFF_SIZE 1000
#define CR 13
#define RANDOM_ASCII rand() % 94 + 32
#define BUFF_SIZE_IN_FXXXFUNC 64

int pid;

void MYError(char * str, FILE * fp, int fd)
{
	printf("%s", str);
	
	close(fd);
	myfclose(fp);
	
	return;
}

/* myfwrite Test Case */
void test_case_1()
{
	FILE * fp;
	int i, fd;
	int endPos = 0;
	char writeBuff[BUFF_SIZE] = { '\0' };
	char readBuff[BUFF_SIZE] = { '\0' };
	int nread, nwrite = 0;
	int nBlocks;
	int tmp;

	if (!(fp = myfopen(TC__FILE_NAME, "w+")))
	{
		printf("TC_1 : myfopen error(write)\n");
		
		return;
	}

	if ((fd = open(TC__FILE_NAME, O_RDONLY)) == -1)
	{
		printf("TC_1 : Open error\n");
		
		myfclose(fp);
		
		return;
	}

	srand(time(NULL));

	for (i = 0; i < BUFF_SIZE - 1; i++)
	{
		if ((i + 1) % 50 == 0)
			writeBuff[i] = CR;
			
		else
			writeBuff[i] = RANDOM_ASCII;
	}

	endPos = BUFF_SIZE - 1;
	nBlocks = endPos / BLOCK_SIZE;

	for (i = 0; i < nBlocks ; i++)
	{
		tmp = myfwrite(writeBuff + nwrite, sizeof(char), BLOCK_SIZE, fp);
		
		if (tmp != BLOCK_SIZE)
		{
			MYError("TC_1 : fwrite error in loop \n", fp, fd);
			
			return;
		}
		
		nwrite += tmp;

		if (nBlocks % 2 == 0)
		{
			if (nwrite == lseek(fd, 0, SEEK_END))
			{
				MYError("TC_1 : error. before myfflush \n", fp, fd);
				
				return;
			}
		}

		myfflush(fp);

		if (nwrite != lseek(fd, 0, SEEK_END))
		{
			MYError("TC_1 : error. after myfflush \n", fp, fd);
			
			return;
		}
	}

	if (endPos % BLOCK_SIZE)
	{
		tmp = myfwrite(writeBuff + nwrite, sizeof(char), endPos % BLOCK_SIZE, fp);
		
		if (tmp != (endPos % BLOCK_SIZE))
		{
			MYError("TC_1 : fwrite error at out of loop \n", fp, fd);
			
			return;
		}
		
		nwrite += tmp;
	}
	
	myfflush(fp);
	
	lseek(fd, 0, SEEK_SET);
	nread = read(fd, readBuff, endPos);
	
	if (nread == -1)
	{
		MYError("TC_1 : read error\n", fp, fd);
		
		return;
	}
		
	if ((strlen(readBuff) != strlen(writeBuff)) || (strncmp(readBuff, writeBuff, endPos) != 0))
	{
		MYError("TC_1 : not the same content with the original _FILE\n", fp, fd);
		
		return;
	}	

	myfclose(fp);
	close(fd);
	
	printf("TC_1 : Success!\n");
	
	return;
}

/* myfread Test Case */
void test_case_2()
{
	FILE * fp;
	int i, fd;
	int endPos = 0;
	char writeBuff[BUFF_SIZE] = { '\0' };
	char readBuff[BUFF_SIZE] = { '\0' };

	if ((fd = open(TC__FILE_NAME, O_WRONLY)) == -1)
	{
		printf("TC_2 : Open error\n");
		
		return;
	}

	if (!(fp = myfopen(TC__FILE_NAME, "w+")))
	{
		printf("TC_2 : myfopen error\n");
		
		close(fd);
		
		return;
	}

	srand(time(NULL));

	for (i = 0; i < BUFF_SIZE - 1; i++)
	{
		if (i != 0 && i % 50 == 0)
			writeBuff[i] = CR;
			
		else
			writeBuff[i] = RANDOM_ASCII;
	}

	endPos = BUFF_SIZE - 1;
	
	if (endPos != write(fd, writeBuff, endPos))
	{
		MYError("TC_2 : write error\n", fp, fd);
		
		return;
	}

	myfread(readBuff, sizeof(char), endPos, fp);

	if ((strlen(readBuff) != strlen(writeBuff)) || (strncmp(readBuff, writeBuff, endPos) != 0))
	{
		MYError("TC_2 : not the same content with the original _FILE\n", fp, fd);
		
		return;
	}
	
	close(fd);	
	myfclose(fp);
	
	printf("TC_2 : Success!\n");
	
	return;		
}

/* Final Testcase */
void test_case_3()
{
	int i;
	FILE * fp, * fp2;
	char tempBuff[BUFF_SIZE] = { '\0' };
	char s1[] = "system software\n";
	char s2[] = "this is the final test case\n";
	int nS1, nS2;
	
	nS1 = strlen(s1);
	nS2 = strlen(s2);

	memcpy(tempBuff, s1, nS1);
	memcpy(tempBuff+nS1, s2, nS2);

	if (!(fp = myfopen(TC__FILE_NAME, "a+")))
	{
		printf("TC_3 : _fopen error\n");
		
		return;
	}

	myfwrite(s1, sizeof(char), nS1, fp);
	myfflush(fp);
	myfclose(fp);

	if (!(fp = myfopen(TC__FILE_NAME, "a+")))
	{
		printf("TC_3 : _fopen error\n");
		
		return;
	}

	myfwrite(s2, sizeof(char), nS1, fp);
	myfflush(fp);
	myfclose(fp);

	printf("TC_3 : Success!\n");

	return;
}

/* main */
int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		printf("Usage : ./client <Server's PID>\n");
		
		return 1;
	}
	
	pid = atoi(argv[1]);
	
	Init();
	test_case_1();
	test_case_2();
	test_case_3();
	
	return 0;
}