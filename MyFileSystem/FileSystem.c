#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileSystem.h"
#include "Buffer.h"

/* declaration */
int  GetBitmap(int index, char * pBitmap);
void SetBitmap(int index, char * pBitmap);
void UnsetBitmap(int index, char * pBitmap);
int  FirstFitBitmap(char * pBitmap);

void GetData(int dataNum, void * pData, size_t size);
int  PutData(void * pData, size_t size);
void DeleteData(int dataNum);
void UpdateData(int dataNum, void * pData, size_t size);

void GetInode(int inodeNum, InodeInfo * pInode);
int  PutInode(int size, FileType fileType, FileMode fileMode, int blocks);
void DeleteInode(int inodeNum);
void UpdateInode(int inodeNum, InodeInfo inode);

int GetDirEntry(int inodeNum, DirEntry * pDirEntry);
int PutDirEntry(int inodeNum, DirEntry dirEntry);
int DeleteDirEntry(int inodeNum, const char * pDirPath);

int AddFileDesc(int inodeNum);
int RemoveFileDesc(int index);
int FindFileDesc(int inodeNum);

void FileSysInfoSync(void);

int    CheckExist(const char * pDirPath);
char * ParseParentPath(const char * pDirPath, char * pPath);
char * ParseFileName(const char * pDirPath, char * pFileName);

FileDescTable * pFileDescTable = NULL;

void FileSysInit(void)
{
    pFileDescTable = (FileDescTable *)malloc(sizeof(FileDescTable));
    memset(pFileDescTable, 0, sizeof(FileDescTable));

    BufInit();

    return;
}

void FileSysFinish(void)
{
    BufSync();
    BufFinish();

    free(pFileDescTable);

    return;
}

void EnumerateDirStatus(const char * pDirPath, DirEntry * pDirEntry, int * pCount)
{
    int      i = 0;
    int      inodeNum = 0;
    int      countOfDir = 0;
    DirEntry arrDirEntry[NUM_OF_DIRECT_BLK_PTR * NUM_OF_DIRENT_PER_BLK] = { 0, };

    inodeNum = CheckExist(pDirPath);

    if (inodeNum < 0)
    {
        *pCount = -1;

        return;
    }

    countOfDir = GetDirEntry(inodeNum, arrDirEntry);

    if (countOfDir > *pCount)
        countOfDir = *pCount;
    else
        *pCount = countOfDir;

    for (i = 0; i < countOfDir; i++)
        pDirEntry[i] = arrDirEntry[i];

    return;
}

int MakeDir(const char * pDirPath)
{
    int       i = 0;
    int       inodeNum = 0;
    int       parent_inodeNum = 0;
    char      name[NAME_LEN_MAX] = { 0, };
    char      path[NAME_LEN_MAX * 3] = { 0, };
    InodeInfo inodeInfo = { 0, };
    DirEntry  dirEntry = { 0, };
    DirEntry  arrDirEntry[NUM_OF_DIRENT_PER_BLK] = { 0, };

    if (CheckExist(pDirPath) > 0)
        return -1;

    ParseParentPath(pDirPath, path);
    parent_inodeNum = CheckExist(path);

    if (parent_inodeNum < 0)
        return -1;

    inodeNum = PutInode(0, FILE_TYPE_DIR, 0, 0);

    ParseFileName(pDirPath, name);
    dirEntry.fileType = FILE_TYPE_DIR;
    strcpy(dirEntry.name, name);
    dirEntry.inodeNum = inodeNum;

    PutDirEntry(parent_inodeNum, dirEntry);

    return 0;
}

int RemoveDir(const char * pDirPath)
{
    int       inodeNum = 0;
    int       parent_inodeNum = 0;
    char      name[NAME_LEN_MAX] = { 0, };
    char      path[NAME_LEN_MAX] = { 0, };
    InodeInfo inodeInfo = { 0, };
    InodeInfo parent_inodeInfo = { 0, };
    DirEntry  dirEntry[NUM_OF_DIRENT_PER_BLK] = { 0, };
    DirEntry  arrDirEntry[NUM_OF_DIRECT_BLK_PTR * NUM_OF_DIRENT_PER_BLK] = { 0, };

    if ((strcmp(pDirPath, "") == 0) || (strcmp(pDirPath, "/") == 0))
        return -1;

    inodeNum = CheckExist(pDirPath);

    if (inodeNum < 0)
        return -1;

    if (GetDirEntry(inodeNum, arrDirEntry) > 0)
        return -1;

    ParseParentPath(pDirPath, path);
    parent_inodeNum = CheckExist(path);
    GetInode(parent_inodeNum, &parent_inodeInfo);

    DeleteInode(inodeNum);

    ParseFileName(pDirPath, name);
    DeleteDirEntry(parent_inodeNum, name);

    return 0;
}

int OpenFile(const char * pFileName, OpenFlag openFlag)
{
    int       inodeNum = 0;
    int       parent_inodeNum = 0;
    char      name[NAME_LEN_MAX] = { 0, };
    char      path[NAME_LEN_MAX] = { 0, };
    InodeInfo inodeInfo = { 0, };
    InodeInfo parent_inodeInfo = { 0, };
    DirEntry  dirEntry = { 0, };

    if (openFlag == OPEN_FLAG_CREATE)
    {
        inodeNum = CheckExist(pFileName);

        if (inodeNum < 0)
        {
            inodeNum = PutInode(0, FILE_TYPE_FILE, 0, 0);

            ParseFileName(pFileName, name);

            dirEntry.fileType = FILE_TYPE_FILE;
            strcpy(dirEntry.name, name);
            dirEntry.inodeNum = inodeNum;

            ParseParentPath(pFileName, path);
            parent_inodeNum = CheckExist(path);

            if (PutDirEntry(parent_inodeNum, dirEntry) < 0)
                return -1;
        }
    }
    else if (openFlag == OPEN_FLAG_READWRITE)
    {
        inodeNum = CheckExist(pFileName);

        if (inodeNum < 0)
            return -1;
    }

    AddFileDesc(inodeNum);

    return 0;
}

int CloseFile(int fd)
{
    RemoveFileDesc(fd);

    return 0;
}

int ReadFile(int fd, char * pData, int length)
{
    int       i = 0, j = 0;
    int       position = 0;
    int       inodeNum = 0;
    char      block[BLOCK_SIZE] = { 0, };
    InodeInfo inodeInfo = { 0, };

    inodeNum = pFileDescTable->file[fd].inodeNum;

    if (!inodeNum > 0)
        return -1;

    GetInode(inodeNum, &inodeInfo);

    for (i = 0; i < inodeInfo.blocks; i++)
    {
        GetData(inodeInfo.directPtr[i], block, sizeof(char) * BLOCK_SIZE);

        for (j = 0; j < BLOCK_SIZE; j++)
        {
            pData[position++] = block[j];

            if (position == length)
                return position;
        }
    }

    return -1;
}

int WriteFile(int fd, char * pData, int length)
{
    int       i = 0, j = 0;
    int       position = 0;
    int       inodeNum = 0;
    char      block[BLOCK_SIZE] = { 0, };
    InodeInfo inodeInfo = { 0, };
    
    inodeNum = pFileDescTable->file[fd].inodeNum;

    if (!inodeNum > 0)
        return -1;

    GetInode(inodeNum, &inodeInfo);

    for (i = 0; i < inodeInfo.blocks; i++)
        DeleteData(inodeInfo.directPtr[i]);

    inodeInfo.blocks = length / BLOCK_SIZE;

    if ((length % BLOCK_SIZE) > 0)
        inodeInfo.blocks++;

    for (i = 0; i < inodeInfo.blocks; i++)
    {
        for (j = 0; j < BLOCK_SIZE; j++)
        {
            block[j] = pData[position++];

            if (position == length)
                break;
        }

        inodeInfo.directPtr[i] = PutData(block, sizeof(char) * BLOCK_SIZE);
    }

    inodeInfo.size = position;

    UpdateInode(inodeNum, inodeInfo);

    return length;
}

int RemoveFile(const char * pFileName)
{
    int       i = 0;
    int       inodeNum = 0;
    int       parent_inodeNum = 0;
    char      name[NAME_LEN_MAX * 3] = { 0, };
    InodeInfo inodeInfo = { 0, };


    inodeNum = CheckExist(pFileName);

    if (inodeNum < 0)
        return -1;

    if (FindFileDesc(inodeNum) >= 0)
        return -1;

    GetInode(inodeNum, &inodeInfo);

    for (i = 0; i < inodeInfo.blocks; i++)
        DeleteData(inodeInfo.directPtr[i]);

    DeleteInode(inodeNum);

    ParseParentPath(pFileName, name);
    parent_inodeNum = CheckExist(name);

    ParseFileName(pFileName, name);
    DeleteDirEntry(parent_inodeNum, name);

    return 0;
}

/* definition */
int GetBitmap(int index, char * pBitmap)
{
    int mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

    if (pBitmap[index / 8] & mask[index % 8])
        return 1;
    else
        return 0;
}

void SetBitmap(int index, char * pBitmap)
{
    int mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

    pBitmap[index / 8] |= mask[index % 8];

    return;
}

void UnsetBitmap(int index, char * pBitmap)
{
    int mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

    pBitmap[index / 8] &= ~mask[index % 8];

    return;
}

int FirstFitBitmap(char * pBitmap)
{
    int i = 0;
    int mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

    for (i = 0; i < FS_INODE_COUNT; i++)
    {
        if (GetBitmap(i, pBitmap) == 0)
            return i;
    }

    return -1;
}

void GetData(int dataNum, void * pData, size_t size)
{
    BufRead(pFileSysInfo->dataStart + dataNum, pData, size);

    return;
}

int PutData(void * pData, size_t size)
{
    int dataNum = FirstFitBitmap(pFileSysInfo->pBlockBitmap);

    BufWrite(pFileSysInfo->dataStart + dataNum, pData, size);
    BufSync();

    pFileSysInfo->numAllocBlocks++;
    pFileSysInfo->numFreeBlocks--;
    SetBitmap(dataNum, pFileSysInfo->pBlockBitmap);
    FileSysInfoSync();

    return dataNum;
}

void DeleteData(int dataNum)
{
    BufWrite(pFileSysInfo->dataStart + dataNum, 0, 0);
    BufSync();

    pFileSysInfo->numAllocBlocks--;
    pFileSysInfo->numFreeBlocks++;
    SetBitmap(dataNum, pFileSysInfo->pBlockBitmap);
    FileSysInfoSync();

    return;
}

void UpdateData(int dataNum, void * pData, size_t size)
{
    BufWrite(pFileSysInfo->dataStart + dataNum, pData, size);
    BufSync();

    return;
}

void GetInode(int inodeNum, InodeInfo * pInode)
{
    int       blockNum = inodeNum / NUM_OF_INODE_PER_BLK;
    int       offset = inodeNum % NUM_OF_INODE_PER_BLK;
    InodeInfo arrInodeInfo[NUM_OF_INODE_PER_BLK] = { 0, };

    BufRead(pFileSysInfo->inodeListStart + blockNum, arrInodeInfo, sizeof(InodeInfo) * NUM_OF_INODE_PER_BLK);
    memcpy(pInode, &arrInodeInfo[offset], sizeof(InodeInfo));

    return;
}

int PutInode(int size, FileType fileType, FileMode fileMode, int blocks)
{
    int       inodeNum = FirstFitBitmap(pFileSysInfo->pInodeBitmap);
    int       blockNum = inodeNum / NUM_OF_INODE_PER_BLK;
    int       offset = inodeNum % NUM_OF_INODE_PER_BLK;
    InodeInfo inodeInfo = { 0, };
    InodeInfo arrInodeInfo[NUM_OF_INODE_PER_BLK] = { 0, };

    inodeInfo.fileMode = fileMode;
    inodeInfo.fileType = fileType;
    inodeInfo.size = size;
    inodeInfo.blocks = blocks;
    memset(inodeInfo.directPtr, 0, sizeof(int) * NUM_OF_DIRECT_BLK_PTR);

    BufRead(pFileSysInfo->inodeListStart + blockNum, arrInodeInfo, sizeof(InodeInfo) * NUM_OF_INODE_PER_BLK);
    arrInodeInfo[offset] = inodeInfo;
    
    BufWrite(pFileSysInfo->inodeListStart + blockNum, arrInodeInfo, sizeof(InodeInfo) * NUM_OF_INODE_PER_BLK);
    BufSync();

    pFileSysInfo->numAllocInodes++;
    pFileSysInfo->numFreeInodes--;
    SetBitmap(inodeNum, pFileSysInfo->pInodeBitmap);
    FileSysInfoSync();

    return inodeNum;
}

void DeleteInode(int inodeNum)
{
    int       i = 0;
    int       blockNum = inodeNum / NUM_OF_INODE_PER_BLK;
    int       offset = inodeNum % NUM_OF_INODE_PER_BLK;
    InodeInfo inodeInfo = { 0, };
    InodeInfo arrInodeInfo[NUM_OF_INODE_PER_BLK] = { 0, };

    GetInode(inodeNum, &inodeInfo);

    for (i = 0; i < inodeInfo.blocks; i++)
        DeleteData(inodeInfo.directPtr[i]);

    BufRead(pFileSysInfo->inodeListStart + blockNum, arrInodeInfo, sizeof(InodeInfo) * NUM_OF_INODE_PER_BLK);
    arrInodeInfo[offset] = inodeInfo;

    BufWrite(pFileSysInfo->inodeListStart + blockNum, arrInodeInfo, sizeof(InodeInfo) * NUM_OF_INODE_PER_BLK);
    BufSync();

    pFileSysInfo->numAllocInodes--;
    pFileSysInfo->numFreeInodes++;
    SetBitmap(inodeNum, pFileSysInfo->pInodeBitmap);
    FileSysInfoSync();

    return;
}

void UpdateInode(int inodeNum, InodeInfo inode)
{
    int       blockNum = inodeNum / NUM_OF_INODE_PER_BLK;
    int       offset = inodeNum % NUM_OF_INODE_PER_BLK;
    InodeInfo arrInodeInfo[NUM_OF_INODE_PER_BLK] = { 0, };

    memset(arrInodeInfo, 0, sizeof(InodeInfo) * NUM_OF_INODE_PER_BLK);

    BufRead(pFileSysInfo->inodeListStart + blockNum, arrInodeInfo, sizeof(InodeInfo) * NUM_OF_INODE_PER_BLK);
    arrInodeInfo[offset] = inode;

    BufWrite(pFileSysInfo->inodeListStart + blockNum, arrInodeInfo, sizeof(InodeInfo) * NUM_OF_INODE_PER_BLK);
    BufSync();

    return;
}

int GetDirEntry(int inodeNum, DirEntry * pDirEntry)
{
    int       i = 0;
    int       count = 0;
    int       countOfDir = 0;
    int       blockNum = 0;
    InodeInfo inodeInfo = { 0, };
    DirEntry  dirEntry[NUM_OF_DIRENT_PER_BLK] = { 0, };

    GetInode(inodeNum, &inodeInfo);

    if (inodeInfo.fileType == FILE_TYPE_FILE)
        return -1;

    countOfDir = inodeInfo.size / sizeof(DirEntry);

    while (count < countOfDir)
    {
        GetData(inodeInfo.directPtr[blockNum++], dirEntry, sizeof(DirEntry) * NUM_OF_DIRENT_PER_BLK);

        for (i = 0; i < NUM_OF_DIRENT_PER_BLK; i++)
        {
            if (dirEntry[i].inodeNum > 0)
            {
                memcpy(&pDirEntry[count], &dirEntry[i], sizeof(DirEntry));
                
                count++;
            }
        }
    }

    return count;
}

int PutDirEntry(int inodeNum, DirEntry dirEntry)
{
    int       i = 0, j = 0;
    int       dataNum = 0;
    InodeInfo inodeInfo = { 0, };
    DirEntry  arrDirEntry[NUM_OF_DIRENT_PER_BLK] = { 0, };

    GetInode(inodeNum, &inodeInfo);

    for (i = 0; i < inodeInfo.blocks; i++)
    {
        GetData(inodeInfo.directPtr[i], arrDirEntry, sizeof(DirEntry) * NUM_OF_DIRENT_PER_BLK);

        for (j = 0; j < NUM_OF_DIRENT_PER_BLK; j++)
        {
            if (arrDirEntry[i].inodeNum == 0)
            {
                arrDirEntry[j] = dirEntry;
                UpdateData(inodeInfo.directPtr[i], arrDirEntry, sizeof(DirEntry) * NUM_OF_DIRENT_PER_BLK);

                inodeInfo.size += sizeof(DirEntry);
                UpdateInode(inodeNum, inodeInfo);

                return 0;
            }
        }
    }

    if (i == NUM_OF_DIRECT_BLK_PTR)
        return -1;

    memset(arrDirEntry, 0, sizeof(DirEntry) * NUM_OF_DIRENT_PER_BLK);
    arrDirEntry[0] = dirEntry;
    dataNum = PutData(arrDirEntry, sizeof(DirEntry) * NUM_OF_DIRENT_PER_BLK);
    
    inodeInfo.size += sizeof(DirEntry);
    inodeInfo.blocks++;
    inodeInfo.directPtr[inodeInfo.blocks - 1] = dataNum;

    UpdateInode(inodeNum, inodeInfo);

    return 0;
}

int DeleteDirEntry(int inodeNum, const char * pDirPath)
{
    int       i = 0, j = 0;
    InodeInfo inodeInfo = { 0, };
    DirEntry  arrDirEntry[NUM_OF_DIRENT_PER_BLK] = { 0, };

    GetInode(inodeNum, &inodeInfo);

    inodeInfo.size -= sizeof(DirEntry);

    for (i = 0; i < inodeInfo.blocks; i++)
    {
        GetData(inodeInfo.directPtr[i], arrDirEntry, sizeof(DirEntry) * NUM_OF_DIRENT_PER_BLK);

        for (j = 0; j < NUM_OF_DIRENT_PER_BLK; j++)
        {
            if (strcmp(arrDirEntry[j].name, pDirPath) == 0)
            {
                memset(arrDirEntry[j].name, 0, NAME_LEN_MAX);
                arrDirEntry[j].fileType = 0;
                arrDirEntry[j].inodeNum = 0;
                
                UpdateData(inodeInfo.directPtr[i], arrDirEntry, sizeof(DirEntry) * NUM_OF_DIRENT_PER_BLK);
                
                UpdateInode(inodeNum, inodeInfo);

                return 0;
            }
        }
    }

    return -1;
}

int AddFileDesc(int inodeNum)
{
    int        i = 0;
    FileDesc * pFileDesc = pFileDescTable->file;

    for (i = 0; i < FS_INODE_COUNT; i++)
    {
        if (pFileDesc[i].valid_bit == 0)
        {
            pFileDesc[i].valid_bit = 1;
            pFileDesc[i].offset = inodeNum % NUM_OF_INODE_PER_BLK;
            pFileDesc[i].valid_bit = 1;

            return i;
        }
    }

    return -1;
}

int RemoveFileDesc(int index)
{
    memset(pFileDescTable->file + index, 0, sizeof(FileDesc));
}

int FindFileDesc(int inodeNum)
{
    int i = 0;

    for (i = 0; i < FS_INODE_COUNT; i++)
    {
        if (pFileDescTable->file[i].inodeNum == inodeNum)
            return i;
    }

    return -1;
}

void FileSysInfoSync(void)
{
    FileSysInfo fileSysInfo = { 0, };

    memcpy(&fileSysInfo, pFileSysInfo, sizeof(FileSysInfo));
    fileSysInfo.pBlockBitmap = 0;
    fileSysInfo.pInodeBitmap = 0;

    BufWrite(0, &fileSysInfo, sizeof(FileSysInfo));
    BufWrite(1, pFileSysInfo->pBlockBitmap, strlen(pFileSysInfo->pBlockBitmap));
    BufWrite(2, pFileSysInfo->pInodeBitmap, strlen(pFileSysInfo->pInodeBitmap));
    BufSync();

    return;
}

int CheckExist(const char * pDirPath)
{
    int      i = 0;
    int      countOfDir = 0;
    int      inodeNum = 0;
    char     name[NAME_LEN_MAX * 3] = { 0, };
    DirEntry arrDirEntry[NUM_OF_DIRECT_BLK_PTR * NUM_OF_DIRENT_PER_BLK] = { 0, };
    
    if ((strcmp(pDirPath, "") == 0) || (strcmp(pDirPath, "/") == 0))
        return pFileSysInfo->rootInodeNum;

    ParseParentPath(pDirPath, name);

    if (strcmp(name, "") == 0)
    {
        ParseFileName(pDirPath, name);

        countOfDir = GetDirEntry(pFileSysInfo->rootInodeNum, arrDirEntry);

        for (i = 0; i < countOfDir; i++)
        {
            if (strcmp(arrDirEntry[i].name, name) == 0)
                return arrDirEntry[i].inodeNum;
        }

        return -1;
    }
    else
    {
        inodeNum = CheckExist(name);

        if (inodeNum < 0)
            return -1;

        ParseFileName(pDirPath, name);

        countOfDir = GetDirEntry(inodeNum, arrDirEntry);

        if (countOfDir < 0)
            return -1;

        for (i = 0; i < countOfDir; i++)
        {
            if (!strcmp(arrDirEntry[i].name, name))
                return arrDirEntry[i].inodeNum;
        }

        return -1;
    }
}

char * ParseParentPath(const char * pDirPath, char * pPath)
{
    strcpy(pPath, pDirPath);

    if (strrchr(pPath, '/') == (strchr(pPath, 0) - 1))
        *(strrchr(pPath, '/')) = 0;

    *(strrchr(pPath, '/')) = 0;

    return pPath;
}

char * ParseFileName(const char * pDirPath, char * pFileName)
{
    memcpy(pFileName, strrchr(pDirPath, '/') + 1, (size_t)(strrchr(pDirPath, 0) - strrchr(pDirPath, '/')));

    return pFileName;
}
