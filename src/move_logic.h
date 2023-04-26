#ifndef _AMAZON_MOVE_LOGIC_H_
#define _AMAZON_MOVE_LOGIC_H_

#include "board.h"
#include "dir.h"
#include <stdbool.h>


typedef struct queen_moves{
    unsigned int *indexes;  //
    unsigned int move_count;
} queen_moves_t;


/// @brief Updates the given queen moves structure and fill the array with the indexes of the possible position of a queen
/// @param board The game board
/// @param moves The pointer on the queen_move struct to fill 
/// @param queen_index The index of the queen 
/// @pre moves->indexes should be of correct size (board size, can even be smaller because is should only hold at max all the possible destination for a queen)
void queen_available_moves(board_t *board, queen_moves_t *moves, unsigned int queen_index);

void king_available_moves(board_t *board, queen_moves_t *moves, unsigned int queen_index);

bool queen_can_move(board_t *board, unsigned int queen_board_index);
bool is_game_over_for_player(board_t *board, unsigned int player);

enum dir_t get_move_direction(board_t *client, unsigned int origin, unsigned int destination);
int compute_step_toward_direction(enum dir_t direction, unsigned int board_width);
bool is_move_valid(board_t *board, struct move_t *move, unsigned int player_id);
int get_queen_liberty(board_t *board, unsigned int queen_board_index);

void display_move(struct move_t m);

#endif // _AMAZON_MOVE_LOGIC_H_
