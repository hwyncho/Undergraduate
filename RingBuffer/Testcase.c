#define _GNU_SOURCE
#include <assert.h>
#include <fcntl.h>
#include <sched.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "MyQueue.h"

#define FLAGS (SIGCHLD | CLONE_VM | CLONE_SIGHAND | CLONE_FS | CLONE_FILES)
#define STACK_SIZE	(1024 * 8)

void testcase();
int produce(void * arg);
int consume(void * arg);

pid_t pid_producer = 0;
pid_t pid_consumer = 0;

int produce_time = 0;
int consume_time = 0;

int main(int argc, char const * argv[]) {
	int testcase_num = 0;
	printf("<< Testcase 번호 입력 (1 or 2 or 3): ");
	scanf("%d", &testcase_num);

	switch (testcase_num) {
		case 1:
			produce_time = 3;
			consume_time = 2;
			break;

		case 2:
			produce_time = 2;
			consume_time = 2;
			break;

		case 3:
			produce_time = 2;
			consume_time = 3;
			break;

		default:
			puts("잘못된 입력값 입니다.");
			return -1;
	}

	srand(time(NULL));
	init();
	testcase();

	while (1)
	{
		sleep(produce_time + consume_time);
		printQueue();
	}

	return 0;
}

void testcase()
{
	char * pStack_producer = (char *)malloc(sizeof(char) * STACK_SIZE);
	char * pStack_consumer = (char *)malloc(sizeof(char) * STACK_SIZE);

	pid_producer = clone(produce, (char *)pStack_producer + STACK_SIZE, FLAGS, &produce_time);
	pid_consumer = clone(consume, (char *)pStack_consumer + STACK_SIZE, FLAGS, &consume_time);
}

int produce(void * arg)
{
	unsigned int * sleep_time = arg;

	while (1)
	{
		enQueue(rand() % 100 + 1);
		sleep(*sleep_time);
	}

	return 0;
}

int consume(void * arg)
{
	unsigned int * sleep_time = arg;

	while (1)
	{
		deQueue();
		sleep(*sleep_time);
	}

	return 0;
}
