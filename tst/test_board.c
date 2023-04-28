#include "board.h"
#include "graph.h"
#include "queens.h"
#include <assert.h>
#include <stdbool.h>

// TODO: Add a graph check
int test_board_create(void) {

    unsigned board_size = 5;
    struct graph_t* graph = create_graph(board_size, SQUARE);
    unsigned int* queens[2];
    queens[0] = queens_create_positions(board_size, 0);
    queens[1] = queens_create_positions(board_size, 1);
    unsigned int queens_number = queens_compute_number(board_size);

    struct board* board = board_create(graph, queens, queens_number);

    assert(board->board_width == 5);
    assert(board->board_cells == 25);
    assert(board->arrows_count == 0);
    for (unsigned int i = 0; i < board->board_cells; i++)
    {
        assert(board->arrows[i] == false);
    }
    assert(board->queens_count == queens_number);
    for (unsigned int i = 0; i < board->queens_count; i++)
    {
        assert(board->queens[0][i] == queens[0][i]);
        assert(board->queens[1][i] == queens[1][i]);
    }

    board_free(board);
    return 0;
}

int test_board_add_arrow(void) {
    
    unsigned board_size = 5;
    struct graph_t* graph = create_graph(board_size, SQUARE);
    unsigned int* queens[2];
    queens[0] = queens_create_positions(board_size, 0);
    queens[1] = queens_create_positions(board_size, 1);
    unsigned int queens_number = queens_compute_number(board_size);

    struct board* board = board_create(graph, queens, queens_number);
    assert(board_add_arrow(board, 0) == true);
    assert(board->arrows[0] == true);
    for (unsigned int i = 1; i < board->board_cells; i++) {
        assert(board->arrows[i] == false);
    }

    assert(board_add_arrow(board, 0) == false);
    assert(board->arrows[0] == true);
    for (unsigned int i = 1; i < board->board_cells; i++) {
        assert(board->arrows[i] == false);
    }

    assert(board_add_arrow(board, board->board_cells+1) == false);
    assert(board->arrows[0] == true);
    for (unsigned int i = 1; i < board->board_cells; i++) {
        assert(board->arrows[i] == false);
    }

    board_free(board);
    return 0;
}