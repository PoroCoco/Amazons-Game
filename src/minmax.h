#ifndef _AMAZON_MINMAX_H
#define _AMAZON_MINMAX_H

#include "board.h"
#include "move_logic.h"
#include "move.h"

typedef struct node{
    struct node **childs;
    struct node *parent;
    size_t child_count;
    size_t allocated;
    struct move_t move;
} node_t;

bool isLeaf(node_t *node);
void add_child(node_t *node, node_t *child);
void remove_child(node_t *node, node_t child);
node_t *create_empty_node(node_t *parent);
void destroy_tree(node_t *root);
void display_node(node_t *node);

node_t *create_moves_tree(board_t *board, unsigned int current_player, unsigned int depth, node_t *root);
double minmax(node_t *node, unsigned int depth, bool maxiPlayer, board_t *board, unsigned int current_player, unsigned int original_player);
struct move_t get_move_minmax(node_t *game_tree, board_t *board, unsigned int current_player);


#endif // _AMAZON_MINMAX_H