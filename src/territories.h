#include"move_logic.h"
#include"board.h"
#include"queue.h"

/// \cond
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
/// \endcond

unsigned int* get_territory_queen_move(board_t* board, unsigned int client);

unsigned int* get_territory_king_move(board_t* board, unsigned int client);
