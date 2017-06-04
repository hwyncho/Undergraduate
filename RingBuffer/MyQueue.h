#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

#define MAX_BUF_NUM 10

int * Queue;
int front;
int rear;

void init(void);
void enQueue(const int data);
void deQueue(void);
void clearQueue(void);
void printQueue(void);
int isEmpty(void);
int isFull(void);

#endif /* __MYQUEUE_H__ */
