#ifndef _AMAZON_MOVE_LOGIC_H_
#define _AMAZON_MOVE_LOGIC_H_

#include "board.h"
#include "dir.h"
/// \cond
#include <stdbool.h>
/// \endcond


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

/**
 * @brief Updates the given queen moves structure and fill the array with the indexes of the possible position of a queen 1 tile away from her.
 * 
 * @param board The current board
 * @param moves The pointer on the queen_move struct to fill 
 * @param queen_index Queen index
 */
void king_available_moves(board_t *board, queen_moves_t *moves, unsigned int queen_index);

/**
 * @brief This function check if a queen is able to move
 * 
 * @param board The current board
 * @param queen_board_index Queen index
 * @return true if the given queen can move, false otherwise
 */
bool queen_can_move(board_t *board, unsigned int queen_board_index);

/**
 * @brief This function check if a game is over
 * 
 * @param board The current board
 * @param player Player id
 * @return true if the player lost, false otherwise
 * @return false
 */
bool is_game_over_for_player(board_t *board, unsigned int player);

/**
 * @brief Get the move direction in order to go from the given index to the destination index
 * 
 * @param client A board stuctur
 * @param origin An origin index
 * @param destination A destination index
 * @return enum dir_t 
 */
enum dir_t get_move_direction(board_t *client, unsigned int origin, unsigned int destination);

enum dir_t compute_get_move_direction(board_t *board, unsigned int origin, unsigned int destination);

/**
 * @brief This function compute an integer to add in order to go in the given destination
 * 
 * @param direction A direction
 * @param board_width The current board width 
 * @return int 
 */
int compute_step_toward_direction(enum dir_t direction, unsigned int board_width);

/**
 * @brief This function check if a given move is valid 
 * 
 * @param board The current board
 * @param move The move
 * @param player_id The player id
 * @return true if the move is valid, false otherwise
 * @return false 
 */
bool is_move_valid(board_t *board, struct move_t *move, unsigned int player_id);
/**
 * @brief This function compute ths amount of available moves for a given queen
 * 
 * @param board The current board
 * @param queen_board_index The queen index
 * @return int 
 */
int get_queen_liberty(board_t *board, unsigned int queen_board_index);

/**
 * @brief This function compute the amount of available moves for a given player
 * 
 * @param board The current board
 * @param player The player id
 * @return unsigned int 
 */
unsigned int possible_moves_count(board_t *board, unsigned int player);

#endif // _AMAZON_MOVE_LOGIC_H_
