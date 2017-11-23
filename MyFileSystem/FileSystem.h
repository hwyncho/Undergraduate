#pragma once

#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Disk.h"

#define FS_DISK_CAPACITY      8388608        // 8MB
#define FS_INODE_COUNT        128
#define NUM_OF_INODE_PER_BLK  (BLOCK_SIZE / sizeof(InodeInfo))
#define NUM_OF_DIRENT_PER_BLK (BLOCK_SIZE / sizeof(DirEntry))
#define MAX_INDEX_OF_DIRBLK   NUM_OF_DIRENT_PER_BLK
#define NAME_LEN_MAX          56
#define NUM_OF_DIRECT_BLK_PTR 12

typedef enum __mountType
{
    MT_TYPE_FORMAT,
    MT_TYPE_READWRITE
} MountType;

typedef enum __openFlag
{
    OPEN_FLAG_CREATE,
    OPEN_FLAG_READWRITE
} OpenFlag;

typedef enum __fileMode
{
    FILE_MODE_READONLY,
    FILE_MODE_READWRITE,
    FILE_MODE_EXEC
} FileMode;

typedef enum __fileType
{
    FILE_TYPE_FILE,
    FILE_TYPE_DIR,
    FILE_TYPE_DEV
} FileType;

typedef struct __inodeInfo
{
    FileMode fileMode;
    FileType fileType;
    int      size;
    int      blocks;                                   // count of blocks
    int      directPtr[NUM_OF_DIRECT_BLK_PTR];        // direct block pointers
} InodeInfo;

typedef struct __fileSysInfo
{
    int    blockSize;               // size of block
    int    diskCapacity;            // capacity of disk
    int    numAllocBlocks;          // count of allocated blocks
    int    numFreeBlocks;           // count of free blocks
    int    numInodes;               // count of inodes
    int    numAllocInodes;          // count of allocated inodes
    int    numFreeInodes;           // count of free inodes
    int    blockBitmapStart;        // start block number of block bitmap
    int    inodeBitmapStart;        // start block number of inode bitmap
    int    inodeListStart;          // start block number of inode list
    int    rootInodeNum;            // block number of root inodes
    int    dataStart;               // start block number of data region
    char * pBlockBitmap;            // pointer of block bitmap
    char * pInodeBitmap;            // pointer of inode bitmap
} FileSysInfo;

typedef struct __fileDesc
{
    int valid_bit;
    int offset;
    int inodeNum;
} FileDesc;

typedef struct __fileDescTable
{
    FileDesc file[FS_INODE_COUNT];
} FileDescTable;

typedef struct __dirEntry
{
    FileType fileType;
    char     name[NAME_LEN_MAX];
    int      inodeNum;
} DirEntry;

typedef struct __dirBlock
{
    DirEntry dirEntries[NUM_OF_DIRENT_PER_BLK];
} DirBlock;

extern void Mount(MountType mountType);
extern void Unmount(void);
extern void FileSysInit(void);
extern void FileSysFinish(void);
extern void EnumerateDirStatus(const char * pDirPath, DirEntry * pDirEntry, int * pCount);
extern int  MakeDir(const char * pDirPath);
extern int  RemoveDir(const char * pDirPath);
extern int  OpenFile(const char * pFileName, OpenFlag openFlag);
extern int  CloseFile(int fd);
extern int  ReadFile(int fd, char * pData, int length);
extern int  WriteFile(int fd, char * pData, int length);
extern int  RemoveFile(const char * pFileName);

extern FileSysInfo * pFileSysInfo;
extern FileDescTable * pFileDescTable;

#endif // !__FILESYSTEM_H__
