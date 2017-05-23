#ifndef GRAPHPARTITIONING_NODE_H
#define GRAPHPARTITIONING_NODE_H

#define MAXNODE 500

typedef struct _Node
{
	int id;
	int degree;
	int * otherId;
} Node;

Node * Graph;

void initGraph(void);
void freeGraph(void);
Node createNode(char * line);
void insertNode(int index, Node node);

#endif //GRAPHPARTITIONING_NODE_H
