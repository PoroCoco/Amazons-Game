#ifndef _AMAZON_MOVE_LOGIC_H_
#define _AMAZON_MOVE_LOGIC_H_

#include "board.h"
#include "dir.h"
#include <stdbool.h>


bool queen_can_move(board_t *board, unsigned int queen_board_index);
bool is_game_won(board_t *board);

enum dir_t get_move_direction(board_t *client, unsigned int origin, unsigned int destination);
int compute_step_toward_direction(enum dir_t direction, unsigned int board_width);
bool is_move_valid(board_t *board, struct move_t *move, unsigned int player_id);

#endif // _AMAZON_MOVE_LOGIC_H_
