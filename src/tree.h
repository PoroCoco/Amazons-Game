#ifndef _THREE_H_
#define _THREE_H_

#include "board.h"
#include "move.h"

struct node
{
    int nb_wins;
    int nb_game;

    struct board *current_board;
    struct move_t *move;

    struct node *parent;
    unsigned int childs_count;
    struct node **childs;
};

struct node *node_create(int win, int game,
                         struct board *current_board,
                         struct move_t *previous_move);

int node_add(struct node *parent, struct node *node);

void node_free(struct node *node);

#endif