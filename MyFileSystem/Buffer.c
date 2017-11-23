#define _GNU_SOURCE

#include <assert.h>
#include <fcntl.h>
#include <sched.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "Buffer.h"
#include "Disk.h"

#define HASH_NUM      (blockNum % HASH_TBL_SIZE)
#define FLAGS         (SIGCHLD | CLONE_VM | CLONE_SIGHAND | CLONE_FS | CLONE_FILES)
#define STACK_SIZE    (1024 * 8)

/* declaration */
int   BufDaemon(void * arg);
void  CheckThreshold(void);
void  InsertBufIntoBufList(int blockNum, BufList listNum, Buf * pBuf);
void  DeleteBufFromBufList(Buf * pBuf);
void  InsertBufIntoLruList(Buf * pBuf);
void  DeleteBufFromLruList(Buf * pBuf);
void  InsertBufIntoBufFreeList(Buf * pBuf);
Buf * FindBuf(int blockNum);
Buf * GetLruBuf(void);
Buf * GetNewBuf(void);

int        countOfFreeBuf = 0;
pid_t    pid_main = 0;
pid_t    pid_daemon = 0;
sem_t    semid_main;
sem_t    semid_daemon;

void BufInit(void)
{
    int    i = 0;
    char * pStack = (char *)malloc(sizeof(char) * STACK_SIZE);
    Buf *  pBuf = NULL;

    /* initialize the pointer of buffer list to NULL */
    for (i = 0; i < MAX_BUFLIST_NUM; i++)
    {
        ppBufListHead[i] = NULL;
        ppBufListTail[i] = NULL;
    }

    /* initialize the pointer of hash table to NULL */
    for (i = 0; i < HASH_TBL_SIZE; i++)
    {
        ppHashHead[i] = NULL;
        ppHashTail[i] = NULL;
    }

    /* initialize the pointer of lru list to NULL */
    pLruListHead = NULL;
    pLruListTail = NULL;

    /* create buffer free list */
    for (i = 0; i < MAX_BUF_NUM; i++)
    {
        pBuf = (Buf *)malloc(sizeof(Buf));
        InsertBufIntoBufFreeList(pBuf);

        pBuf = NULL;
    }

    pid_main = getpid();
    pid_daemon = clone(BufDaemon, (char *)pStack + STACK_SIZE, FLAGS, NULL);        // create a bufdaemon thread

    sem_init(&semid_main, 0, 1);
    sem_init(&semid_daemon, 0, 0);

    sem_wait(&semid_main);

    return;
}

void BufFinish(void)
{
    kill(pid_daemon, SIGTERM);
    waitpid(pid_daemon, NULL);

    sem_destroy(&semid_daemon);
    sem_destroy(&semid_main);

    return;
}

void BufRead(int blockNum, void * pData, size_t size)
{
    Buf * pBuf = FindBuf(blockNum);

    if (pBuf == NULL)        // there is not a buffer having the blockNum
    {
        pBuf = GetNewBuf();
        pBuf->state = BUF_STATE_CLEAN;
        pBuf->blockNum = blockNum;
        DevReadBlock(blockNum, pBuf->data);
        
        InsertBufIntoBufList(blockNum, BUF_LIST_CLEAN, pBuf);
    }

    DeleteBufFromLruList(pBuf);
    InsertBufIntoLruList(pBuf);

    memcpy(pData, pBuf->data, size);

    return;
}

void BufWrite(int blockNum, void * pData, size_t size)
{
    Buf * pBuf = FindBuf(blockNum);

    if (pBuf == NULL)        // there is not a buffer having the blockNum
    {
        pBuf = GetNewBuf();
        pBuf->state = BUF_STATE_DIRTY;
        pBuf->blockNum = blockNum;
        pBuf->aTime = 0;
        memcpy(pBuf->data, pData, size);

        InsertBufIntoBufList(blockNum, BUF_LIST_DIRTY, pBuf);
    }
    else                    // there is a buffer having the blockNum
    {
        if (pBuf->state == BUF_STATE_CLEAN)
        {
            DeleteBufFromBufList(pBuf);

            pBuf->state = BUF_STATE_DIRTY;
            memcpy(pBuf->data, pData, size);

            InsertBufIntoBufList(blockNum, BUF_LIST_DIRTY, pBuf);
        }
        else
            memcpy(pBuf->data, pData, size);
    }

    DeleteBufFromLruList(pBuf);
    InsertBufIntoLruList(pBuf);

    return;
}

void BufSync(void)
{
    Buf * pBuf = ppBufListHead[BUF_LIST_DIRTY];

    while (pBuf != NULL)
    {
        DevWriteBlock(pBuf->blockNum, pBuf->data);

        DeleteBufFromBufList(pBuf);

        pBuf->state = BUF_STATE_CLEAN;

        InsertBufIntoBufList(pBuf->blockNum, BUF_LIST_CLEAN, pBuf);

        pBuf = ppBufListHead[BUF_LIST_DIRTY];
    }

    return;
}

void  GetBufInfoByListNum(BufList listNum, Buf ** ppBufInfo, int * pCountOfBuf)
{
    int   count = 0;
    Buf * pBuf = ppBufListHead[listNum];

    while (pBuf != NULL)
    {
        ppBufInfo[count] = pBuf;
        count++;

        pBuf = pBuf->pbNext;
    }

    *pCountOfBuf = count;

    return;
}

void GetBufInfoInLruList(Buf ** ppBufInfo, int * pCountOfBuf)
{
    int   count = 0;
    Buf * pBuf = pLruListHead;

    while (pBuf != NULL)
    {
        ppBufInfo[count] = pBuf;
        count++;

        pBuf = pBuf->plNext;
    }

    *pCountOfBuf = count;

    return;
}

/* definition */
int BufDaemon(void * arg)
{
    Buf * pBuf = NULL;

    while (1)
    {
        sem_wait(&semid_daemon);

        /* buffer replacement */
        do
        {
            pBuf = GetLruBuf();

            if (pBuf->state == BUF_STATE_DIRTY)
                DevWriteBlock(pBuf->blockNum, pBuf->data);

            DeleteBufFromBufList(pBuf);
            InsertBufIntoBufFreeList(pBuf);
        } while (countOfFreeBuf < (int)(MAX_BUF_NUM * 0.3));

        sem_post(&semid_main);
    }

    return 0;
}

void CheckThreshold(void)
{
    if (countOfFreeBuf == (int)(MAX_BUF_NUM * 0.1))
    {
        sem_post(&semid_daemon);

        sem_wait(&semid_main);
    }

    return;
}

void InsertBufIntoBufList(int blockNum, BufList listNum, Buf * pBuf)
{
    pBuf->blockNum = blockNum;

    /* insert the buffer to the tail of buffer list */
    if (ppBufListTail[listNum] == NULL)        // a case that the buffer list is empty
    {
        ppBufListHead[listNum] = ppBufListTail[listNum] = pBuf;
        ppBufListHead[listNum]->pbPrev = ppBufListTail[listNum]->pbNext = NULL;
    }
    else                                    // a case that the buffer list has buffer at least one
    {
        ppBufListTail[listNum]->pbNext = pBuf;
        pBuf->pbPrev = ppBufListTail[listNum];

        ppBufListTail[listNum] = pBuf;
        ppBufListTail[listNum]->pbNext = NULL;
    }

    /* insert the buffer to the tail hash table */
    if (ppHashTail[HASH_NUM] == NULL)        // a case that the hash table is empty
    {
        ppHashHead[HASH_NUM] = ppHashTail[HASH_NUM] = pBuf;
        ppHashHead[HASH_NUM]->phPrev = ppHashTail[HASH_NUM]->phNext = NULL;
    }
    else                                    // a case that the hash table has buffer at least one
    {
        ppHashTail[HASH_NUM]->phNext = pBuf;
        pBuf->phPrev = ppHashTail[HASH_NUM];

        ppHashTail[HASH_NUM] = pBuf;
        ppHashTail[HASH_NUM]->phNext = NULL;
    }

    return;
}

void DeleteBufFromBufList(Buf * pBuf)
{
    int   i = 0;
    Buf * pbPrev = NULL;
    Buf * pbNext = NULL;
    Buf * phPrev = NULL;
    Buf * phNext = NULL;

    /* delete the buffer from buffer list */
    if ((pBuf->pbPrev == NULL) && (pBuf->pbNext == NULL))        // a case that the buffer is the only one member of buffer list
    {
        for (i = 0; i < (MAX_BUFLIST_NUM - 1); i++)
        {
            if (pBuf == ppBufListHead[i])
                ppBufListHead[i] = ppBufListTail[i] = NULL;
        }
    }
    else if (pBuf->pbPrev == NULL)                                // a case that buffer is the head of buffer list
    {
        pbNext = pBuf->pbNext;

        for (i = 0; i < (MAX_BUFLIST_NUM - 1); i++)
        {
            if (pBuf == ppBufListHead[i])
            {
                ppBufListHead[i] = pbNext;
                ppBufListHead[i]->pbPrev = NULL;
            }
        }
    }
    else if (pBuf->pbNext == NULL)                                // a case that buffer is the tail of buffer list
    {
        pbPrev = pBuf->pbPrev;

        for (i = 0; i < (MAX_BUFLIST_NUM - 1); i++)
        {
            if (pBuf == ppBufListTail[i])
            {
                ppBufListTail[i] = pbPrev;
                ppBufListTail[i]->pbNext = NULL;
            }
        }
    }
    else
    {
        pbPrev = pBuf->pbPrev;
        pbNext = pBuf->pbNext;

        pbPrev->pbNext = pbNext;
        pbNext->pbPrev = pbPrev;
    }

    /* delete the buffer from hash table */
    if ((pBuf->phPrev == NULL) && (pBuf->phNext == NULL))        // a case that the buffer is the only one member of buffer list
    {
        for (i = 0; i < HASH_TBL_SIZE; i++)
        {
            if (pBuf == ppHashHead[i])
                ppHashHead[i] = ppHashTail[i] = NULL;
        }
    }
    else if (pBuf->phPrev == NULL)                                // a case that buffer is the head of buffer list
    {
        phNext = pBuf->phNext;

        for (i = 0; i < HASH_TBL_SIZE; i++)
        {
            if (pBuf == ppHashHead[i])
            {
                ppHashHead[i] = phNext;
                ppHashHead[i]->phPrev = NULL;
            }
        }
    }
    else if (pBuf->phNext == NULL)                                // a case that buffer is the tail of buffer list
    {
        phPrev = pBuf->phPrev;

        for (i = 0; i < HASH_TBL_SIZE; i++)
        {
            if (pBuf == ppHashHead[i])
            {
                ppHashTail[i] = phPrev;
                ppHashTail[i]->phNext = NULL;
            }
        }
    }
    else
    {
        phPrev = pBuf->phPrev;
        phNext = pBuf->phNext;

        phPrev->phNext = phNext;
        phNext->phPrev = phPrev;
    }

    pBuf->pbPrev = NULL;
    pBuf->pbNext = NULL;
    pBuf->phPrev = NULL;
    pBuf->phNext = NULL;

    return;
}

void InsertBufIntoLruList(Buf * pBuf)
{
    pBuf->aTime++;

    if (pLruListHead == NULL)
    {
        pLruListHead = pLruListTail = pBuf;
        pLruListHead->plPrev = pLruListTail->plNext = NULL;
    }
    else
    {
        pLruListHead->plPrev = pBuf;
        pBuf->plNext = pLruListHead;

        pLruListHead = pBuf;
        pLruListHead->plPrev = NULL;
    }

    return;
}

void DeleteBufFromLruList(Buf * pBuf)
{
    Buf * plPrev = NULL;
    Buf * plNext = NULL;

    if ((pBuf->plPrev == NULL) && (pBuf->plNext == NULL))        // a case that the buffer is not in lru list
        return;
    else if (pBuf->plPrev == NULL)                                // a case that buffer is the head of lru list
    {
        plNext = pBuf->plNext;

        pLruListHead = plNext;
        pLruListHead->plPrev = NULL;
    }
    else if (pBuf->plNext == NULL)                                // a case that buffer is the tail of lru list
    {
        plPrev = pBuf->plPrev;

        pLruListTail = plPrev;
        pLruListTail->plNext = NULL;
    }
    else
    {
        plPrev = pBuf->plPrev;
        plNext = pBuf->plNext;

        plPrev->plNext = plNext;
        plNext->plPrev = plPrev;
    }

    pBuf->plPrev = NULL;
    pBuf->plNext = NULL;

    return;
}

void InsertBufIntoBufFreeList(Buf * pBuf)
{
    free(pBuf->data);

    memset(pBuf, 0, sizeof(Buf));
    pBuf->state = 2;
    pBuf->blockNum = BLKNUM_INVALID;
    pBuf->aTime = 0;
    pBuf->data = (void *)malloc(BLOCK_SIZE);
    memset(pBuf->data, 0, BLOCK_SIZE);

    if (ppBufListTail[BUF_LIST_FREE] == NULL)        // a case that the buffer free list is empty
    {
        ppBufListHead[BUF_LIST_FREE] = ppBufListTail[BUF_LIST_FREE] = pBuf;
        ppBufListHead[BUF_LIST_FREE]->pbPrev = ppBufListTail[BUF_LIST_FREE]->pbNext = NULL;
    }
    else                                            // a case that the buffer free list has node at least one
    {
        ppBufListTail[BUF_LIST_FREE]->pbNext = pBuf;
        pBuf->pbPrev = ppBufListTail[BUF_LIST_FREE];

        ppBufListTail[BUF_LIST_FREE] = pBuf;
        ppBufListTail[BUF_LIST_FREE]->pbNext = NULL;
    }

    countOfFreeBuf++;

    return;
}

Buf * FindBuf(int blockNum)
{
    int   i = 0;
    Buf * pBuf = NULL;

    CheckThreshold();

    /* search the hash table from 0 to END */
    for (i = 0; i < HASH_TBL_SIZE; i++)
    {
        pBuf = ppHashHead[i];

        while (pBuf != NULL)
        {
            if (pBuf->blockNum == blockNum)
                return pBuf;
            else
                pBuf = pBuf->phNext;
        }
    }

    return NULL;
}

Buf * GetLruBuf(void)
{
    Buf * prev = NULL;
    Buf * tail = NULL;

    if (pLruListTail == NULL)
        return NULL;
    else if (pLruListTail == pLruListHead)
    {
        tail = pLruListTail;

        pLruListHead = pLruListTail = NULL;
    }
    else
    {
        prev = pLruListTail->plPrev;
        tail = pLruListTail;

        pLruListTail = prev;
        pLruListTail->plNext = NULL;
    }

    return tail;
}

Buf * GetNewBuf(void)
{
    Buf * next = NULL;
    Buf * head = NULL;

    if (ppBufListHead[BUF_LIST_FREE] == NULL)                                    // a case that the object list is empty
        return NULL;
    else if (ppBufListHead[BUF_LIST_FREE] == ppBufListTail[BUF_LIST_FREE])        // a case that the object has a node
    {
        head = ppBufListHead[BUF_LIST_FREE];

        ppBufListHead[BUF_LIST_FREE] = ppBufListTail[BUF_LIST_FREE] = NULL;
    }
    else                                                                        // a case that the object list has nodes
    {
        next = ppBufListHead[BUF_LIST_FREE]->pbNext;
        head = ppBufListHead[BUF_LIST_FREE];

        ppBufListHead[BUF_LIST_FREE] = next;
        ppBufListHead[BUF_LIST_FREE]->pbPrev = NULL;
    }

    head->blockNum = BLKNUM_INVALID;

    countOfFreeBuf--;

    return head;
}
