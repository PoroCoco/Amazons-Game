#ifndef _TERRITORIES_H_
#define _TERRITORIES_H_


#include"move_logic.h"
#include"board.h"
#include"queue.h"

/// \cond
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
/// \endcond

struct territory_cell{
    unsigned int distance;
    unsigned int queens_index;
};


struct territory_cell* get_territory_queen_move(board_t* board, unsigned int client, struct queue * queue);
struct territory_cell* get_territory_king_move(board_t* board, unsigned int client, struct queue * queue);

#endif 