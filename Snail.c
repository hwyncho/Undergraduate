/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code implements snail array.
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:11
 */
#include <stdio.h>

#define MAX 100

void shape(int (* ptr)[MAX], int size, int course);
void print(int (* ptr)[MAX], int size);

int main(void)
{
    int size;
    int arr[MAX][MAX];

    printf("<<< Input size : ");
    scanf("%d", &size);

    shape(arr, size, 1);
    print(arr, size);

    return 0;
}

void shape(int (* ptr)[MAX], int size, int course)
{
    int i;
    static int x = -1, y = 0;
    static int num = 1;

    if (size == 0)
        return;

    for (i = 0; i < size * 2 - 1; i++)
    {
        if (i < size)
            x += course;

        else
            y += course;

        ptr[y][x] = num++;
    }

    shape(ptr, size - 1, -course);
}

void print(int (* ptr)[MAX], int size)
{
    int i, j;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
            printf("%2d ", ptr[i][j]);

        printf("\n");
    }
}
