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

/**
 * @brief This function create and allocate a node on the heap
 * 
 * @param win amount of child's wins
 * @param game number of games played by the childs
 * @param current_board 
 * @param previous_move 
 * @return struct node* 
 */
struct node *node_create(int win, int game,
                         struct board *current_board,
                         struct move_t *previous_move);

/**
 * @brief This function add a child node to a parent node
 * 
 * @param parent 
 * @param node 
 * @return int return 1 if node has been added to parents childs array
 */
int node_add(struct node *parent, struct node *node);

/**
 * @brief This function free a given tree
 * 
 * @param node 
 */
void node_free(struct node *node);

#endif