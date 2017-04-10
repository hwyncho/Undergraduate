#pragma once

#ifndef __BUFFER_H__
#define __BUFFER_H__

#define HASH_TBL_SIZE	8
#define MAX_BUFLIST_NUM	3
#define MAX_BUF_NUM		10
#define BLKNUM_INVALID	-1

typedef struct __buf Buf;

typedef enum _bool
{
	FALSE = 0,
	TRUE = 1
} BOOL;

typedef enum __bufList
{
	BUF_LIST_CLEAN = 0,
	BUF_LIST_DIRTY = 1,
	BUF_LIST_FREE = 2
} BufList;

typedef enum _bufState
{
	BUF_STATE_CLEAN = 0,
	BUF_STATE_DIRTY = 1
} BufState;

struct __buf
{
	BufState	state;
	int			blockNum;
	int			aTime;
	void *		data;
	Buf *		pbPrev;		// the previous pointer in buffer list
	Buf *		pbNext;		// the next pointer in buffer list
	Buf *		phPrev;		// the previous pointer in hash table
	Buf *		phNext;		// the next pointer in hash table
	Buf *		plPrev;		// the previous pointer in lru(least recently used) list
	Buf *		plNext;		// the next pointer in lru(least recently used) list
};

extern void BufInit(void);
extern void BufFinish(void);
extern void BufRead(int blockNum, void * pData, size_t size);
extern void BufWrite(int blockNum, void * pData, size_t size);
extern void BufSync(void);
extern void GetBufInfoByListNum(BufList listNum, Buf ** ppBufInfo, int * pCountOfBuf);
extern void GetBufInfoInLruList(Buf ** ppBufInfo, int * pCountOfBuf);

Buf * ppBufListHead[MAX_BUFLIST_NUM];
Buf * ppBufListTail[MAX_BUFLIST_NUM];
Buf * ppHashHead[HASH_TBL_SIZE];
Buf * ppHashTail[HASH_TBL_SIZE];
Buf * pLruListHead;
Buf * pLruListTail;

#endif // !__BUFFER_H__