#ifndef _AMAZON_ALPHABETA_H_
#define _AMAZON_ALPHABETA_H_

#include "board.h"
#include "move_logic.h"
#include "move.h"


double alphabeta(double alpha, double beta, board_t *board, unsigned int current_player, bool maxiPlayer, unsigned int original_player, unsigned int depth);
struct move_t get_move_alphabeta(board_t *board, unsigned int current_player);




#endif // _AMAZON_ALPHABETA_H_
