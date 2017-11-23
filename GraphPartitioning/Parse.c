#include <stdio.h>
#include <string.h>
#include "Parse.h"
#include "Graph.h"

void parseGraph(const char * path)
{
    int i = 0;
    char line[MAXCOUNT] = { 0x00 };
    FILE * file = fopen(path, "r");

    if (file != NULL)
    {
        initGraph();

        while (!feof(file) && (i < MAXNODE))
        {
            memset(line, 0x00, MAXCOUNT);
            fgets(line, MAXCOUNT, file);

            insertNode(i++, createNode(line));
        }

        fclose(file);
    }
    else
    {
        perror("[Error] void parse(const char * path)");
        return;
    }
}
