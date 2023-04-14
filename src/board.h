#ifndef _AMAZON_BOARD_H_
#define _AMAZON_BOARD_H_

#include "graph_ext.h"
#include "player.h"
#include <stdbool.h>

typedef struct board {
    struct graph_t *g;
    unsigned int board_width;
    unsigned int board_cells;
    bool *arrows;
    unsigned int arrows_count;
    unsigned int *queens[NUM_PLAYERS];
    unsigned int queens_count;
} board_t;

enum cell_state{
    STATE_AVAILABLE,
    STATE_QUEEN_WHITE,
    STATE_QUEEN_BLACK,
    STATE_ARROW,
    STATE_ERROR
};

/// @brief Allocates on the heap a game board.
/// @param g The graph given by the server
/// @param queens The array of array containing the queens positions 
/// @param queens_count The number of queens per player
/// @return A pointer on the board
board_t * board_create(struct graph_t *g, unsigned int *queens[NUM_PLAYERS], unsigned int queens_count);

/// @brief Adds an arrow to the board at the position index.
/// @param b The board
/// @param index The position to block
bool board_add_arrow(board_t *board, unsigned int index);

/// @brief Removes an arrow on the board at the position index. There must be an arrow to remove at the given index.
/// @param b The board
/// @param index The position to unblock
void board_remove_arrow(board_t *board, unsigned int index);

/// @brief Returns the availability state of an index. False if outside of grid or blocked by arrow or blocked by a queen.
/// @param board The game board
/// @param index The position aimed
/// @return True if the cell is empty, false otherwise  
bool board_index_is_available(board_t *board, unsigned int index);


bool board_index_is_available_from(board_t *board, unsigned int source, unsigned int dest);

/// @brief Gives the state of a board cell at the given index. Returns STATE_ERROR in case of errors (for exemple invalid index)
/// @param board The game board
/// @param index The cell index
/// @return The state of the cell at the given index
enum cell_state board_get_index_state(board_t *board, unsigned int index);


void board_get_directions(void);


/// @brief Get the board width. The board is square so the width is equal to the height.
/// @param board The game board
/// @return The board width
unsigned int board_width(board_t *board);

/// @brief Frees all the memory allocated for the board.
/// @param board The game board
void board_free(board_t *board);

/// @brief Display an ASCII representation of the current state of the game board.
/// @param board The game board
void board_print(board_t *board);


#endif // _AMAZON_BOARD_H





