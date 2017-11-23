#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Disk.h"

int fd = 0;
int countOfDiskRead = 0;
int countOfDiskWrite = 0;

/* declaration */
void DevMoveBlock(int blockNum);

void DevCreateDisk(void)
{
    fd = open("MY_DISK", O_RDWR | O_CREAT | O_TRUNC, 0644);

    return;
}

void DevOpenDisk(void)
{
    fd = open("MY_DISK", O_RDWR, 0644);

    return;
}

void DevCloseDisk(void)
{
    close(fd);
    
    return;
}

void DevReadBlock(int blockNum, void * pData)
{
    DevMoveBlock(blockNum);

    read(fd, pData, BLOCK_SIZE);

    countOfDiskRead++;

    return;
}

void DevWriteBlock(int blockNum, void * pData)
{
    DevMoveBlock(blockNum);

    write(fd, pData, BLOCK_SIZE);

    countOfDiskWrite++;

    return;
}

int DevGetDiskReadCount(void)
{
    return countOfDiskRead;
}

int DevGetDiskWriteCount(void)
{
    return countOfDiskWrite;
}

void DevResetDiskAccessCount(void)
{
    countOfDiskRead = countOfDiskWrite = 0;

    return;
}

/* definition */
void DevMoveBlock(int blockNum)
{
    lseek(fd, (off_t)+(BLOCK_SIZE * blockNum), SEEK_SET);

    return;
}
