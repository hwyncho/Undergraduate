#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include "FileSystem.h"
#include "Buffer.h"
#include "Disk.h"

#define DIR_NUM_MAX 100

extern FileSysInfo * pFileSysInfo;

void TestCase1(void);
void TestCase2(void);
void TestCase3(void);
void TestCase4(void);
void TestCase5(void);
void PrintBlockBitmap(void);
void PrintInodeBitmap(void);
void __GetInode(int inodenNum, InodeInfo * pInodeInfo);
void ListDirContents(const char * pDirPath);
void ListDirContentsAndSize(const char * pDirPath);

int main(int argc, char ** argv)
{
	int testcaseNum = 0;

	if (argc < 3)
	{
	ERROR:
		printf("usage : a.out [format | readwrite] [1 ~ 5]\n");

		return -1;
	}

	if (strcmp(argv[1], "format") == 0)
		Mount(MT_TYPE_FORMAT);
	else if (strcmp(argv[1], "readwrite") == 0)
		Mount(MT_TYPE_READWRITE);
	else
		goto ERROR;

	testcaseNum = atoi(argv[2]);

	DevResetDiskAccessCount();

	switch (testcaseNum)
	{
	case 1:
		TestCase1();
		break;

	case 2:
		TestCase2();
		break;

	case 3:
		TestCase3();
		break;

	case 4:
		TestCase4();
		break;

	case 5:
		TestCase5();
		break;

	default:
		Unmount();
		goto ERROR;
		break;
	}

	PrintInodeBitmap();
	PrintBlockBitmap();

	Unmount();

	printf(": The number of disk access counts is %d\n", DevGetDiskReadCount() + DevGetDiskWriteCount());

	return 0;
}

void TestCase1(void)
{
	int		i = 0;
	char	dirPath[NAME_LEN_MAX];

	printf(" ---- Test Case 1 ----\n");

	MakeDir("/tmp");
	MakeDir("/usr");
	MakeDir("/etc");
	MakeDir("/home");

	/* make home directory */
	for (i = 0; i < 8; i++)
	{
		memset(dirPath, 0, sizeof(char) * NAME_LEN_MAX);
		sprintf(dirPath, "/home/user%d", i);
		MakeDir(dirPath);
	}

	/* make etc directory */
	for (i = 0; i < 24; i++)
	{
		memset(dirPath, 0, sizeof(char) * NAME_LEN_MAX);
		sprintf(dirPath, "/etc/dev%d", i);
		MakeDir(dirPath);
	}

	ListDirContents("/home");
	ListDirContents("/etc");

	/* remove subdirectory of etc directory */
	for (i = 23; i >= 0; i--)
	{
		memset(dirPath, 0, sizeof(char) * NAME_LEN_MAX);
		sprintf(dirPath, "/etc/dev%d", i);
		RemoveDir(dirPath);
	}

	ListDirContents("/etc");

	return;
}

void TestCase2(void)
{
	int		i = 0, j = 0;
	int		fd = 0;
	char	fileName[NAME_LEN_MAX];
	char	dirPath[NAME_LEN_MAX];

	printf(" ---- Test Case 2 ----\n");

	ListDirContents("/home");

	/* make home directory */
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 9; j++)
		{
			memset(fileName, 0, sizeof(char) * NAME_LEN_MAX);
			sprintf(dirPath, "/home/user%d/file%d", i, j);

			fd = OpenFile(fileName, OPEN_FLAG_CREATE);
			CloseFile(fd);
		}
	}

	for (i = 0; i < 8; i++)
	{
		memset(dirPath, 0, sizeof(char) * NAME_LEN_MAX);
		sprintf(dirPath, "/home/user%d", i);
		ListDirContents(dirPath);
	}

	return;
}

void TestCase3(void)
{
	int		i = 0;
	int		fd = 0;
	char	fileName[NAME_LEN_MAX];
	char	pBuffer1[512];
	char	pBuffer2[512];

	printf(" ---- Test Case 3 ----\n");

	for (i = 0; i < 9; i++)
	{
		memset(fileName, 0, sizeof(char) * NAME_LEN_MAX);
		sprintf(fileName, "/home/user7/file%d", i);

		fd = OpenFile(fileName, OPEN_FLAG_CREATE);

		memset(pBuffer1, 0, 512);
		strcpy(pBuffer1, fileName);

		WriteFile(fd, pBuffer1, 512);
		CloseFile(fd);
	}

	for (i = 0; i < 9; i++)
	{
		memset(fileName, 0, sizeof(char) * NAME_LEN_MAX);
		sprintf(fileName, "/home/user7/file%d", i);

		fd = OpenFile(fileName, OPEN_FLAG_READWRITE);

		memset(pBuffer1, 0, 512);
		strcpy(pBuffer1, fileName);
		
		memset(pBuffer2, 0, 512);
		ReadFile(fd, pBuffer2, 512);

		if (strcmp(pBuffer1, pBuffer2))
		{
			printf("TesetCase 3 : error\n");

			exit(0);
		}

		CloseFile(fd);
	}

	printf(" ---- Test Case 3 : files written/read ----\n");

	ListDirContents("/home/user7");

	return;
}

void TestCase4(void)
{
	int		i = 0;
	int		fd = 0;
	char	fileName[NAME_LEN_MAX];
	char	pBuffer[1024];

	printf(" ---- Test Case 4 ----\n");

	for (i = 0; i < 9; i++)
	{
		if ((i % 2) == 0)
		{
			memset(fileName, 0, NAME_LEN_MAX);
			sprintf(fileName, "/home/user7/file%d", i);
			RemoveFile(fileName);
		}
	}

	printf(" ---- Test Case 4 : files of even number removed ----\n");

	for (i = 0; i < 9; i++)
	{
		if (i % 2)
		{
			memset(fileName, 0, NAME_LEN_MAX);
			sprintf(fileName, "/home/user7/file%d", i);

			fd = OpenFile(fileName, OPEN_FLAG_READWRITE);

			memset(pBuffer, 0, 1024);
			strcpy(pBuffer, fileName);

			WriteFile(fd, pBuffer, 513);
			CloseFile(fd);
		}
	}
	
	printf(" ---- Test Case 4 : files of odd number overwritten ----\n");

	ListDirContents("/home/user7");

	for (i = 0; i < 9; i++)
	{
		if ((i % 2) == 0)
		{
			memset(fileName, 0, NAME_LEN_MAX);
			sprintf(fileName, "/home/user7/file%d", i);

			fd = OpenFile(fileName, OPEN_FLAG_CREATE);

			memset(pBuffer, 0, 1024);
			strcpy(pBuffer, fileName);

			WriteFile(fd, pBuffer, 513);
			WriteFile(fd, pBuffer, 513);
			CloseFile(fd);
		}
	}

	printf(" ---- Test Case 4 : files of even number re-created & written ----\n");

	ListDirContents("/home/user7");

	return;
}

void TestCase5(void)
{
	printf(" ---- Test Case 5 ----\n");

	ListDirContentsAndSize("/home/user7");

	return;
}

void PrintBlockBitmap(void)
{
	int i = 0;
	int count = BLOCK_SIZE / sizeof(int);
	int * pBitmap = (int *)pFileSysInfo->pBlockBitmap;

	printf("Block bitmap : ");

	for (i = 0; i < count; i++)
		printf("%d", pBitmap[i]);

	printf("\n");

	return;
}

void PrintInodeBitmap(void)
{
	int i = 0;
	int count = BLOCK_SIZE / sizeof(int);
	int * pBitmap = (int *)pFileSysInfo->pInodeBitmap;

	printf("Inode bitmap : ");

	for (i = 0; i < count; i++)
		printf("%d", pBitmap[i]);

	printf("\n");

	return;
}

void __GetInode(int inodenNum, InodeInfo * pInodeInfo)
{
	int			blockNum = pFileSysInfo->inodeListStart + (inodenNum / NUM_OF_INODE_PER_BLK);
	int			bitNum = inodenNum % NUM_OF_INODE_PER_BLK;
	InodeInfo *	pInodeInfo2 = NULL;
	
	memset(pInodeInfo2, 0, sizeof(InodeInfo));
	BufRead(blockNum, pInodeInfo2, sizeof(InodeInfo));
	memcpy(pInodeInfo, &pInodeInfo2[bitNum], sizeof(InodeInfo));
}

void ListDirContents(const char * pDirPath)
{
	int			i = 0;
	int			count = 0;
	DirEntry	dirEntry[DIR_NUM_MAX];
	
	memset(dirEntry, 0, sizeof(DirEntry) * DIR_NUM_MAX);
	EnumerateDirStatus(pDirPath, dirEntry, &count);

	printf("[%s] sub-directory:\n", pDirPath);

	for (i = 0; i < count; i++)
	{
		if (dirEntry[i].fileType == FILE_TYPE_FILE)
			printf("\t name : %s, inode no : %d, type : file\n", dirEntry[i].name, dirEntry[i].inodeNum);
		else if (dirEntry[i].fileType == FILE_TYPE_DIR)
			printf("\t name : %s, inode no : %d, type : directory\n", dirEntry[i].name, dirEntry[i].inodeNum);
		else
			assert(0);
	}

	return;
}

void ListDirContentsAndSize(const char * pDirPath)
{
	int			i = 0;
	int			count = 0;
	InodeInfo	inodeInfo;
	DirEntry	dirEntry[DIR_NUM_MAX];

	memset(dirEntry, 0, sizeof(DirEntry) * DIR_NUM_MAX);
	EnumerateDirStatus(pDirPath, dirEntry, &count);

	printf("[%s] sub-directory:\n", pDirPath);

	for (i = 0; i < count; i++)
	{
		if (dirEntry[i].fileType == FILE_TYPE_FILE)
		{
			__GetInode(dirEntry[i].inodeNum, &inodeInfo);
			printf("\t name : %s, inode no : %d, type : file\n", dirEntry[i].name, dirEntry[i].inodeNum);
		}
		else if (dirEntry[i].fileType == FILE_TYPE_DIR)
			printf("\t name : %s, inode no : %d, type : directory\n", dirEntry[i].name, dirEntry[i].inodeNum);
		else
			assert(0);
	}

	return;
}