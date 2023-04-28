#ifndef _AMAZON_CLIENT_MINMAX_H_
#define _AMAZON_CLIENT_MINMAX_H_


#define NUM_PLAYERS 2

#include "move.h"
#include "graph_ext.h"
#include "dir.h"
#include "board.h"
#include <stdbool.h>


typedef struct client{
    char *name;
    int id;
    board_t *board;
} client_t;

#endif // _AMAZON_CLIENT_MINMAX_H_