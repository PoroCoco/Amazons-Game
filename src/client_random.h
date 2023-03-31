#ifndef _CLIENT_RANDOM_H_
#define _CLIENT_RANDOM_H_

#define NUM_PLAYERS 2

#include "move.h"
#include "graph_ext.h"
#include "dir.h"
#include "board.h"
#include <stdbool.h>


typedef struct random_client{
    char *name;
    int id;
    board_t *board;
} random_client_t;

enum dir_t get_move_direction(random_client_t *client, unsigned int origin, unsigned int destination);

bool is_move_valid(random_client_t *c, struct move_t *move);

#endif