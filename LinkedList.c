/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code implements Linked List.
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode
{
	char data[10];
	struct ListNode * link;
} listNode;

typedef struct
{
	listNode * head;
} linkedList_h;

linkedList_h * createLinkedList(void);
void freeLinkedList(linkedList_h * L);
void printList(linkedList_h * L);
void insertFirstNode(linkedList_h * L, char * x);
void deleteFirstNode(linkedList_h * L);
void insertMiddleNode(linkedList_h * L, char * x);
void deleteMiddleNode(linkedList_h * L);
void insertLastNode(linkedList_h * L, char * x);
void deleteLastNode(linkedList_h * L);
void searchNode(linkedList_h * L, char * x);

int	main(void)
{
	linkedList_h * L;

	L = createLinkedList();
	printf(">>> 공백 리스트를 생성했습니다.\n");

	insertFirstNode(L, "월");
	insertLastNode(L, "목");
	insertMiddleNode(L, "수");
	insertMiddleNode(L, "화");

	printf(">>> 리스트 L을 출력합니다.\n\t");
	printList(L);

	printf(">>> 첫번째 노드를 삭제합니다.\n\t");
	deleteFirstNode(L);
	printList(L);

	printf(">>> 중간 노드를 삭제합니다.\n\t");
	deleteMiddleNode(L);
	printList(L);

	printf(">>> 마지막 노드를 삭제합니다.\n\t");
	deleteLastNode(L);
	printList(L);

	printf(">>> \"화\" 노드를 탐색합니다.\n\t");
	searchNode(L, "화");
	printf("\n");

	freeLinkedList(L);
	printf(">>> 리스트 공간을 해제했습니다.\n\t");
	printList(L);

	return	0;
}

linkedList_h * createLinkedList(void)
{
	linkedList_h * L;

	L = (linkedList_h *)malloc(sizeof(linkedList_h));
	L->head = NULL;

	return	L;
}

void freeLinkedList(linkedList_h * L)
{
	listNode * p;

	while (L->head != NULL)
	{
		p = L->head;
		L->head = L->head->link;
		free(p);
		p = NULL;
	}
}

void printList(linkedList_h * L)
{
	listNode * p;

	printf("L = (");

	p = L->head;

	while (p != NULL)
	{
		printf("%s", p->data);

		p = p->link;
		if (p != NULL)
			printf(", ");
	}

	printf(")\n");
}

void insertFirstNode(linkedList_h * L, char * x)
{
	listNode * newNode;

	newNode = (listNode *)malloc(sizeof(listNode));

	strcpy_s(newNode->data, 10, x);
	newNode->link = L->head;
	L->head = newNode;
}

void	deleteFirstNode(linkedList_h * L)
{
	listNode * firstNode;
	listNode * secondNode;

	secondNode = (listNode *)malloc(sizeof(listNode));

	if (L == NULL)
		return;

	else
	{
		firstNode = L->head;
		secondNode = (L->head)->link;
		L->head = secondNode;
	}
}

void insertMiddleNode(linkedList_h * L, char * x)
{
	listNode * previousNode;
	listNode * newNode;

	newNode = (listNode *)malloc(sizeof(listNode));

	strcpy_s(newNode->data, 10, x);

	if (L->head == NULL)
	{
		L->head = newNode;
		newNode->link = NULL;
	}

	else
	{
		previousNode = L->head;
		newNode->link = (L->head)->link;
		previousNode->link = newNode;
	}
}

void deleteMiddleNode(linkedList_h * L)
{
	listNode * previousNode;
	listNode * deleteNode;

	if (L == NULL)
		return;

	else
	{
		previousNode = L->head;
		deleteNode = previousNode->link;
		if (deleteNode == NULL)
			return;

		previousNode->link = deleteNode->link;
	}
}

void insertLastNode(linkedList_h * L, char * x)
{
	listNode * newNode;
	listNode * p;

	newNode = (listNode *)malloc(sizeof(listNode));
	strcpy_s(newNode->data, 10, x);
	newNode->link = NULL;

	if (L->head == NULL)
	{
		L->head = newNode;

		return;
	}

	p = L->head;

	while (p->link != NULL)
	{
		p = p->link;
	}

	p->link = newNode;
}

void deleteLastNode(linkedList_h * L)
{
	listNode * previousNode;
	listNode * LastNode;

	if (L->head == NULL)
		return;

	if ((L->head)->link == NULL)
	{
		free(L->head);
		L->head = NULL;

		return;
	}

	else
	{
		previousNode = L->head;
		LastNode = (L->head)->link;

		while (LastNode->link != NULL)
		{
			previousNode = LastNode;
			LastNode = LastNode->link;
		}
	}

	free(LastNode);
	previousNode->link = NULL;
}

void searchNode(linkedList_h * L, char * x)
{
	listNode *	temp;

	temp = L->head;

	while (temp != NULL)
	{
		if (strcmp(temp->data, x) == 0)
		{
			printf("해당 노드가 존재합니다.");
			break;
		}

		else
			temp = temp->link;
	}
}
