#ifndef _CLIENT_COCKTAIL_H_
#define _CLIENT_COCKTAIL_H_


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

#endif 
