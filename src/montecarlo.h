#ifndef MCTS_H
#define MCTS_H


#define NUM_PLAYERS 2

#include <stdbool.h>
#include "move.h"
#include "graph_ext.h"
#include "dir.h"
#include "board.h"
#include "tree.h"
#include "board.h"
#include "queens.h"
#include "move_logic.h"
#include "heuristic.h"
#include "territories.h"

typedef struct mcts_client{
    char *name;
    int id;
    board_t *board;
} mcts_client;

#endif // MCTS_H
