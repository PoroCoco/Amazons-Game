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
    unsigned int *queens[NUM_PLAYERS];
    unsigned int queens_count;
} board_t;


board_t * board_create(struct graph_t *g, unsigned int *queens[NUM_PLAYERS], unsigned int queens_count);
void board_add_arrow(board_t *b, unsigned int index);
bool board_index_is_available(board_t *b, unsigned int index);
void board_get_directions(void);
unsigned int board_width(board_t *b);
void board_free(board_t *b);
void board_print(board_t *b);


#endif // _AMAZON_BOARD_H





