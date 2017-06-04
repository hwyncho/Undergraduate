#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyQueue.h"

void init(void)
{
	Queue = (int *)malloc(sizeof(int) * MAX_BUF_NUM);
	memset(Queue, -1, sizeof(int) * MAX_BUF_NUM);

	front = 0;
	rear = -1;
}

void enQueue(const int data)
{
	printf("<< enQueue(%d)", data);

	if (isFull() == 1)
	{
		puts(": Queue is full!");
		deQueue();
	}
	else
	{
		rear = (rear + 1) % MAX_BUF_NUM;
		Queue[rear] = data;
		puts("");
	}
}

void deQueue(void)
{
	printf(">> deQueue()");

	if (isEmpty() == 1)
	{
		puts(": Queue is empty!");
	}
	else
	{
		Queue[front] = -1;
		front = (front + 1) % MAX_BUF_NUM;
		puts("");
	}
}

void clearQueue(void)
{
	free(Queue);
}

void printQueue(void)
{
	int	i = 0;
	int _front = front % MAX_BUF_NUM;
	int _rear = rear % MAX_BUF_NUM;

	printf(">> Buffer Status %d %d: [", _front, _rear);

	i = _front;
	while (i != _rear)
	{
		printf("%3d ", Queue[i]);
		i = (i + 1) % MAX_BUF_NUM;
	}
	printf("%3d ] \n", Queue[i]);
}

int isEmpty(void)
{
	if (front == rear)
	{
		return 1;
	}

	return 0;
}

int isFull(void)
{
	if ((rear != -1) && ((rear + 1) % MAX_BUF_NUM) == front)
	{
		return 1;
	}

	return 0;
}
