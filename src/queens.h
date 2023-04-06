#ifndef _AMAZON_QUEENS_H
#define _AMAZON_QUEENS_H

#include <stdbool.h>

/// @brief Compute the number of queens for the given board size
/// @param board_width
/// @return The number of queens that each player should have
unsigned int queens_compute_number(unsigned int board_width);

/// @brief Creates an array with the position of all the queens at the beginning of the game for the given player.
/// @param queens_number The number of queens for this player
/// @param player_id The player
/// @return A heap allocated array of size queens_number containing the position of each queen for the player.
unsigned int* queens_create_positions(unsigned int board_width, unsigned int player_id);

/// @brief Frees the allocated memory to the array of positions
/// @param queens The array to free
void queens_free_positions(unsigned int *queens);


/// @brief Creates a copy of the array of queens
/// @param queens The array to copy
/// @param queen_count The size of queens
/// @return A heap allocated array with the same values as queens
unsigned int* queens_copy(const unsigned int *queens, unsigned int queen_count);

/// @brief Changes the position of a queen
/// @param queens The array to modify
/// @param src The queen to move
/// @param dst The destination
void queens_move(unsigned int *queens, unsigned int src, unsigned int dst);

/// @brief Tests if an position occupied by a queen 
/// @param queens The checked array
/// @param index The position to check
/// @param board_width Size of the boards the @p queens are placed in
/// @return true if queens contains index
bool queens_occupy(const unsigned int *queens, unsigned int index, unsigned int board_width);


#endif // _AMAZON_QUEENS_H
