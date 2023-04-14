#include "test.h"
#include "move_logic.h"
#include "queens.h"


int test_is_move_valid(void){
    int board_width = 8;
    unsigned int *queens[NUM_PLAYERS];
    queens[0] = queens_create_positions(board_width, 0);
    queens[1] = queens_create_positions(board_width, 1);
    board_t *b = board_create(create_graph(board_width, SQUARE), queens, queens_compute_number(board_width));
    
    struct move_t m = {.queen_src = 2, .queen_dst = 20, .arrow_dst = 18};
    int result = is_move_valid(b, &m, 0);

    board_free(b);
    return !result;
}

int test_get_queen_liberty(void) {
    int board_width = 5;
    unsigned int *queens[NUM_PLAYERS];
    queens[0] = queens_create_positions(board_width, 0);
    queens[1] = queens_create_positions(board_width, 1);
    board_t *b = board_create(create_graph(board_width, SQUARE), queens, queens_compute_number(board_width));

    int queens_positions[8] = {1, 3, 5, 9, 15, 19, 21, 23};
    

    for (int i = 0; i < 8; i++) {
        assert(get_queen_liberty(b, queens_positions[i]) == 4);
    }

    board_free(b);
    return 0;
}