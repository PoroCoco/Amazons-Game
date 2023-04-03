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
    board->arrows_count = board->board_width*board->board_width;
    board->arrows = malloc(sizeof(bool)*board->arrows_count);
    for (unsigned int i = 0; i < board->arrows_count; i++)
    {
        board->arrows[i] = false;
    }
    
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

enum cell_state board_get_index_state(board_t *board, unsigned int index){
    //If the index is greater than the grid max 
    if (index >= board->board_cells) return STATE_ERROR;
    if(board->arrows[index]) return STATE_ARROW;
    if(queens_occupy(board->queens[0], index, board->board_width)) return STATE_QUEEN;
    if(queens_occupy(board->queens[1], index, board->board_width)) return STATE_QUEEN;
    return STATE_AVAILABLE;
}



void board_print(board_t *board){
    unsigned int width = board->board_width;

    for (unsigned int i = 0; i < width*2+1; i++)
    {
        printf("-");
    }
    printf("\n");

    for (unsigned int i = 0; i < width; i++)
    {
        for (unsigned int j = 0; j < width; j++){
            printf("|");
            enum cell_state s = board_get_index_state(board, (i*width) + j);
            if (s == STATE_AVAILABLE) printf("_");
            if (s == STATE_ARROW) printf("X");
            if (s == STATE_QUEEN) printf("♕");
            if (s == STATE_ERROR) printf("?");
        }
        printf("|\n");
    }
    
    for (unsigned int i = 0; i < width*2+1; i++)
    {
        printf("-");
    }
    printf("\n");
    
}