#include "board.h"
#include "queens.h"
#include "graph_ext.h"

board_t * board_create(struct graph_t *g, unsigned int *queens[NUM_PLAYERS], unsigned int queens_count){
    board_t *board = malloc(sizeof(*board));
    board->g = g;
    board->queens[0] = queens[0];
    board->queens[1] = queens[1];
    board->queens_count = queens_count;
    board->board_width = (unsigned int) sqrt(g->t->size1);
    board->board_cells = g->num_vertices;
    board->arrows = malloc(sizeof(bool)*board->board_cells);
    
    return board;
}


void board_free(board_t *board){
    destroy_graph(board->g);
    free(board->queens[0]);
    free(board->queens[1]);
    free(board->arrows);
    free(board);
}


unsigned int board_width(board_t *board){
    return board->board_width;
}

bool board_index_is_available(board_t *board, unsigned int index){
    //TODO:CHECK GRAPH VERTEX!!!
    return (index < board->board_cells) && (!board->arrows[index]) 
    && (queens_occupy(board->queens[0], index, board->board_width) && queens_occupy(board->queens[1], index, board->board_width));
}
