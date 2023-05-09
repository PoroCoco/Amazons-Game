#ifndef _AMAZON_HEURISTIC_H_
#define _AMAZON_HEURISTIC_H_

#include "board.h"
#include "move_logic.h"
#include "territories.h"


double power_heuristic(board_t *board, unsigned int player_id);
double power_heuristic_safe(board_t *board, unsigned int player_id);
double territory_heuristic_average(board_t *board, unsigned int player_id,  struct territory_cell* get_array(board_t *, unsigned int));





#endif // _AMAZON_HEURISTIC_H
