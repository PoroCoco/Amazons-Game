#ifndef _CLIENT_RANDOM_H_
#define _CLIENT_RANDOM_H_

#define NUM_PLAYERS 2

#include "move.h"
#include "graph.h"
#include "dir.h"


struct random_client
{
    char *name;
    int id;
    struct graph_t *graph;
    unsigned int nums_queens;
    unsigned int **queens;
};

enum dir_t get_move_direction(size_t origin, size_t destination, size_t width);

int is_move_valid(struct random_client *c, struct move_t *move);

#endif