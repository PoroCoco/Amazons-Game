#ifndef _AMAZON_QUEENS_H
#define _AMAZON_QUEENS_H

#include <stdbool.h>

/// @brief Compute the number of queens for the given board size
/// @return The number of queens that each player should have
unsigned int queens_compute_number(unsigned int board_size);

/// @brief Creates an array with the position of all the queens at the beginning of the game for the given player.
/// @param queens_number The number of queens for this player
/// @param player_id The player
/// @return A heap allocated array of size queens_number containing the position of each queen for the player.
unsigned int* queens_create_positions(unsigned int queens_number, unsigned int player_id);

/// @brief Creates a copy of the array of queens
/// @param queens The array to copy
/// @param queens_number The size of queens
/// @return A heap allocated array with the same values as queens
unsigned int* queens_copy(const unsigned int *queens, unsigned int queens_number);



bool queens_occupy(const unsigned int *queens, unsigned int index, unsigned int board_size);


#endif // _AMAZON_QUEENS_H
