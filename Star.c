/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:12
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int size;
	int coneHeight;
	int body;
	int star;
	int space;

	while (1)
	{
		printf("<<< Please inputu the size of star : ");
		scanf("%d", &size);

		/* △ */
		for (coneHeight = 0; coneHeight < size; coneHeight++)
		{
			for (space = 1; space <= 2 * size - 1; space++)
				printf(" ");

			for (space = size - coneHeight; space > 1; space--)
				printf(" ");

			for (star = 1; star <= 2 * coneHeight + 1; star++)
				printf("*");

			body = star - 1;

			printf("\n");
		}

		/* □ */
		body = (size - 1) * 2 + body;

		/* ＼_/ */
		for (coneHeight = size; coneHeight > 0; coneHeight--)
		{
			for (space = 1; space <= size - coneHeight; space++)
				printf(" ");

			for (star = 1; star <= body + 2 * coneHeight; star++)
				printf("*");

			printf("\n");
		}

		/* /￣＼ */
		if (size == 1)
			coneHeight = 0;

		else
			coneHeight = 1;

		for ( ; coneHeight < size; coneHeight++)
		{
			for (space = size - coneHeight; space > 1; space--)
				printf(" ");

			for (star = 1; star <= body + 2 * coneHeight + 2; star++)
				printf("*");

			printf("\n");
		}

		/* ▽ */
		for (coneHeight = size; coneHeight > 0; coneHeight--)
		{
			for (space = 1; space <= 2 * size - 1; space++)
				printf(" ");

			for (space = size - coneHeight; space >= 1; space--)
				printf(" ");

			for (star = 1; star <= 2 * coneHeight - 1; star++)
				printf("*");

			body = star - 1;

			printf("\n");
		}

		rewind(stdin);
	}

	return 0;
}
