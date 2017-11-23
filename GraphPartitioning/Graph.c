#include "Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initGraph(void)
{
    Graph = (Node *)malloc(sizeof(Node) * MAXNODE);
}

void freeGraph(void)
{
    free(Graph);
}

Node createNode(char * line)
{
    int i = 0;
    int j = 0;
    char * token = strtok(line, " ");

    Node node;
    memset(&node, 0x00, sizeof(Node));

    while (token != NULL)
    {
        switch (i)
        {
            case 0:
                node.id = atoi(token);
                break;

            case 1:
                node.degree = atoi(token);
                break;

            default:
                node.otherId[j++] = atoi(token);
                break;
        }

        token = strtok(NULL, " ");
        i++;
    }

    return node;
}

void insertNode(int index, Node node)
{
    Graph[index] = node;
}
