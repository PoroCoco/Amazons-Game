#include <assert.h>

//node header

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

//header

bool isLeaf(node_t *node){
    return node->child_count == 0;
}


node_t *create_empty_node(node_t *parent){
    node_t *empty_node = malloc(sizeof(*empty_node));
    assert(empty_node);

    empty_node->parent = parent;
    struct move_t move = {-1, -1, -1}; 
    empty_node->move = move;
    empty_node->child_count = 0;
    empty_node->allocated = 128;
    empty_node->childs = malloc(sizeof(*empty_node->childs) * empty_node->allocated);
    assert(empty_node->childs);


    return empty_node;
}

void add_child(node_t *node, node_t *child){
    if(child == NULL || node == child) return;
    if (node->child_count == node->allocated){
        node->allocated *= 2;
        node->childs = realloc(node->childs, sizeof(*node->childs) * node->allocated);
        assert(node->childs);
    }
    child->parent = node;
    node->childs[node->child_count] = child;

    node->child_count++;
}

void destroy_tree(node_t *root){
    assert(root);
    for (size_t i = 0; i < root->child_count; i++)
    {
        destroy_tree(root->childs[i]);
    }

    free(root->childs);
    free(root);
}

void _display_move(struct move_t m){
    printf("{arrow = %u, queen_src = %u, queen_dst = %u}", m.arrow_dst, m.queen_src, m.queen_dst);
}
void display_node(node_t *node){
    printf("node : child_count = %zu, allocated_child = %zu, move =", node->child_count, node->allocated);
    _display_move(node->move);
    printf("\n");
}


node_t *create_moves_tree(board_t *board, unsigned int current_player, unsigned int depth, node_t *root){
    if (depth == 0){
        return NULL;
    }
    if (root == NULL){
        root = create_empty_node(NULL);
    }
    queen_moves_t queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells);
    assert(queen_moves.indexes);

    queen_moves_t arrow_moves;
    arrow_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells);
    assert(arrow_moves.indexes);


    //for every queen of the player
    for (unsigned int i = 0; i < board->queens_count; i++)
    {
        unsigned int queen_source = board->queens[current_player][i];
        queen_available_moves(board, &queen_moves, queen_source);

        //for every position that a queen can move to
        for (unsigned int j = 0; j < queen_moves.move_count; j++)
        {
            unsigned int queen_destination = queen_moves.indexes[j];
            queens_move(board->queens[current_player], board->board_width, queen_source, queen_destination);
            queen_available_moves(board, &arrow_moves, queen_destination);

            //for every position that a moved queen can fire an arrow to
            for (unsigned int k = 0; k < arrow_moves.move_count; k++)
            {
                node_t *new_move_node = create_empty_node(root);
                struct move_t new_move = {  .arrow_dst = arrow_moves.indexes[k],
                                            .queen_src = queen_source,
                                            .queen_dst = queen_destination};
                new_move_node->move = new_move;
                apply_move(board, &new_move, current_player);
                add_child(root, create_moves_tree(board, 1 - current_player, depth - 1, new_move_node));
                undo_move(board, &new_move, current_player);
            }

            //resets board by moving queen back to its old position
            queens_move(board->queens[current_player], board->board_width, queen_destination, queen_source);
        }
    }

    free(queen_moves.indexes);
    free(arrow_moves.indexes);
    return root;
}
