#include"move_logic.h"
#include"limits.h"
#include"board.h"
#include"dir.h"
#include"queue.h"
#include<assert.h>
#include<stdlib.h>

unsigned int* get_territory_queen_move(board_t* board, unsigned int client);

unsigned int* get_territory_king_move(board_t* board, unsigned int client);
