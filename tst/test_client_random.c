#include "client_random.h"
#include "assert.h"

int test_get_move_direction(void) {

    // Testing get_move_direction on a 10 width square graph
    assert(get_move_direction(1, 3, 10) == 7);
    assert(get_move_direction(1, 11, 10) == 5);
    assert(get_move_direction(21, 1, 10) == 1);
    assert(get_move_direction(3, 1, 10) == 3);
    assert(get_move_direction(0, 0, 10) == -1);

    return 0;
} 