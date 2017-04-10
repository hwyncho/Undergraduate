#pragma once

#ifndef __DISK_H__
#define __DISK_H__

#define BLOCK_SIZE (512 * 2)

extern void	DevCreateDisk(void);
extern void	DevOpenDisk(void);
extern void	DevCloseDisk(void);
extern void	DevReadBlock(int blockNum, void * pData);
extern void	DevWriteBlock(int blockNum, void * pData);
extern int	DevGetDiskReadCount(void);
extern int	DevGetDiskWriteCount(void);
extern void	DevResetDiskAccessCount(void);

#endif // !__DISK_H__