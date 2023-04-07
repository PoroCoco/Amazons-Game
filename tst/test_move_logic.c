#include "test.h"
#include "move_logic.h"
#include "queens.h"


int test_is_move_valid(void){
    int board_width = 8;
    unsigned int *queens[NUM_PLAYERS];
    queens[0] = queens_create_positions(board_width, 0);
    queens[1] = queens_create_positions(board_width, 1);
    board_t *b = board_create(create_graph(board_width, SQUARE), queens, queens_compute_number(board_width));
    board_print(b);
    struct move_t m = {.queen_src = 2, .queen_dst = 10, .arrow_dst = 18};
    int result = is_move_valid(b, &m, 0);

    board_free(b);
    return !result;
}
