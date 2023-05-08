#include "tree.h"
#include "board.h"
#include "test.h"
#include "move.h"
#include "queens.h"
#include <assert.h>
#include <stdbool.h>

int test_node_create(void) {
    int board_width = 8;
    unsigned int *queens[NUM_PLAYERS];
    queens[0] = queens_create_positions(board_width, 0);
    queens[1] = queens_create_positions(board_width, 1);
    board_t *b = board_create(create_graph(board_width, SQUARE), queens, queens_compute_number(board_width));
    
    struct move_t *m = malloc(sizeof(*m));
    m->queen_src = 2;
    m->queen_dst = 20;
    m->arrow_dst = 18;

    struct node* node = node_create(0, 0, b, m);

    node_free(node);
    board_free(b);
    return 0;
}

int test_node_add(void) {
    int board_width = 8;
    unsigned int *queens[NUM_PLAYERS];
    queens[0] = queens_create_positions(board_width, 0);
    queens[1] = queens_create_positions(board_width, 1);
    board_t *b = board_create(create_graph(board_width, SQUARE), queens, queens_compute_number(board_width));
    
    struct move_t *first_move = malloc(sizeof(*first_move));
    first_move->queen_src = 2;
    first_move->queen_dst = 20;
    first_move->arrow_dst = 18;

    struct move_t *second_move = malloc(sizeof(*second_move));
    second_move->queen_src = 1;
    second_move->queen_dst = 17;
    second_move->arrow_dst = 16;

    struct node* root = node_create(0, 0, b, first_move);
    struct node* child = node_create(1, 2, b, second_move);

    assert(node_add(root, child) == 1);

    assert(root->childs_count == 1);
    assert(root->childs[0] == child);
    assert(child->parent == root);

    node_free(root);
    board_free(b);
    return 0;
}