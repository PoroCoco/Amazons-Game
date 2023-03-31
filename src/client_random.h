#ifndef _CLIENT_RANDOM_H_
#define _CLIENT_RANDOM_H_

#define NUM_PLAYERS 2

#include "move.h"
#include "graph_ext.h"
#include "dir.h"


struct random_client
{
    char *name;
    int id;
    struct graph_t *graph;
    unsigned int nums_queens;
    unsigned int **queens;
};


#endif