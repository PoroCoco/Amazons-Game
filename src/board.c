#include "board.h"

board_t * board_create(struct graph_t *g, unsigned int *queens[NUM_PLAYERS], unsigned int queens_count){
    board_t *b = malloc(sizeof(*b));
    b->g = g;
    b->queens[0] = queens[0];
    b->queens[1] = queens[1];
    b->queens_count = queens_count;
    b->board_width = (unsigned int) sqrt(g->t->size1);
    b->board_cells = g->num_vertices;
    b->arrows = malloc(sizeof(bool)*b->board_cells);
    
    return b;
}


void board_free(board_t *b){
    destroy_graph(b->g);
    free(b->queens[0]);
    free(b->queens[1]);
    free(b->arrows);
    free(b);
}


unsigned int board_width(board_t *b){
    return b->board_width;
}

bool board_index_is_available(board_t *b, unsigned int index){
    return (!b->arrows[index]); //TODO:CHECK GRAPH VERTEX!!!
}
