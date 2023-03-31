#ifndef _CLIENT_RANDOM_H_
#define _CLIENT_RANDOM_H_

#define NUM_PLAYERS 2

#include "move.h"
#include "graph_ext.h"
#include "dir.h"
#include "board.h"


struct random_client
{
    char *name;
    int id;
    board_t *board;
};

enum dir_t get_move_direction(size_t origin, size_t destination, size_t width);

int is_move_valid(struct random_client *c, struct move_t *move);

#endif