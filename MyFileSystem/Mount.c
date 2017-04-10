#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileSystem.h"
#include "Disk.h"

#define NUM_ALLOC_BLOCKS	4
#define NUM_ALLOC_INODES	1
#define BLOCK_BITMAP_START	1
#define INODE_BITMAP_START	2
#define INODE_LIST_START	3
#define DATA_START			10

FileSysInfo *	pFileSysInfo = NULL;
char			blockBitmap[BLOCK_SIZE] = { 0, };
char			inodeBitmap[BLOCK_SIZE] = { 0, };

void Format(void);
void ReadWrite(void);

void Mount(MountType mountType)
{
	pFileSysInfo = (FileSysInfo *)malloc(sizeof(FileSysInfo));
	memset(pFileSysInfo, 0, sizeof(FileSysInfo));

	switch (mountType)
	{
	case MT_TYPE_FORMAT:
		Format();
		break;

	case MT_TYPE_READWRITE:
		ReadWrite();
		break;
	}

	FileSysInit();

	return;
}

void Unmount(void)
{
	FileSysFinish();

	free(pFileSysInfo);

	DevCloseDisk();

	return;
}

void Format(void)
{
	int			i = 0;
	InodeInfo	inodeInfo = { 0, };

	DevCreateDisk();

	for (i = 0; i < (FS_DISK_CAPACITY / BLOCK_SIZE); i++)
		DevWriteBlock(i, 0x00);

	/* set file system */
	pFileSysInfo->blockSize = BLOCK_SIZE;
	pFileSysInfo->diskCapacity = (int)(FS_DISK_CAPACITY / BLOCK_SIZE);
	pFileSysInfo->numAllocBlocks = NUM_ALLOC_BLOCKS;
	pFileSysInfo->numFreeBlocks = (int)(FS_DISK_CAPACITY / BLOCK_SIZE) - NUM_ALLOC_BLOCKS;
	pFileSysInfo->numInodes = FS_INODE_COUNT;
	pFileSysInfo->numAllocInodes = NUM_ALLOC_INODES;
	pFileSysInfo->numFreeInodes = FS_INODE_COUNT - NUM_ALLOC_INODES;
	pFileSysInfo->blockBitmapStart = BLOCK_BITMAP_START;
	pFileSysInfo->inodeBitmapStart = INODE_BITMAP_START;
	pFileSysInfo->inodeListStart = INODE_LIST_START;
	pFileSysInfo->rootInodeNum = 0;
	pFileSysInfo->dataStart = DATA_START;
	pFileSysInfo->pBlockBitmap = blockBitmap;
	pFileSysInfo->pInodeBitmap = inodeBitmap;

	blockBitmap[0] = 0x01;		// set block bitmap
	inodeBitmap[0] = 0x01;		// set inode bitmap

	/* set root's inode */
	inodeInfo.fileMode = FILE_MODE_READWRITE;
	inodeInfo.fileType = FILE_TYPE_DIR;
	inodeInfo.size = 0;
	inodeInfo.blocks = 0;
	inodeInfo.directPtr[0] = DATA_START;

	DevWriteBlock(0, pFileSysInfo);
	DevWriteBlock(BLOCK_BITMAP_START, blockBitmap);
	DevWriteBlock(INODE_BITMAP_START, inodeBitmap);
	DevWriteBlock(INODE_LIST_START, &inodeInfo);

	return;
}

void ReadWrite(void)
{
	DevOpenDisk();

	DevReadBlock(0, pFileSysInfo);
	DevReadBlock(BLOCK_BITMAP_START, blockBitmap);
	DevReadBlock(INODE_BITMAP_START, inodeBitmap);

	return;
}