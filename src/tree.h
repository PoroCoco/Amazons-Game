#ifndef _AMAZON_TREE_H_
#define _AMAZON_TREE_H_

#include "board.h"
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



#endif // _AMAZON_TREE_H_
