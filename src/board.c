#include "board.h"
#include "queens.h"
#include "graph_ext.h"
#include "move_logic.h"
#include <assert.h>

board_t * board_create(struct graph_t *g, unsigned int *queens[NUM_PLAYERS], unsigned int queens_count){
    board_t *board = malloc(sizeof(*board));
    board->g = g;
    board->queens[0] = queens[0];
    board->queens[1] = queens[1];
    board->queens_count = queens_count;
    board->board_width = (unsigned int) sqrt(g->t->size1);
    board->board_cells = board->board_width*board->board_width;
    board->arrows_count = 0;
    board->arrows = malloc(sizeof(bool)*board->board_cells);
    for (unsigned int i = 0; i < board->board_cells; i++)
    {
        board->arrows[i] = false;
    }

    board->queen_occupy = malloc(sizeof(bool)*board->board_cells);
    for (unsigned int i = 0; i < board->board_cells; i++)
    {
        board->queen_occupy[i] = false;
    }

    for (unsigned int i = 0; i < queens_count; i++)
    {
        for (size_t j = 0; j < NUM_PLAYERS; j++)
        {
            board->queen_occupy[board->queens[j][i]] = true;
        }
    }
    
    board->directions = malloc(sizeof(enum dir_t *) * board->board_cells);
    assert(board->directions);

    for (size_t i = 0; i < board->board_cells; i++)
    {
        board->directions[i] = malloc(sizeof(enum dir_t) * board->board_cells);
        assert(board->directions[i]);
        for (size_t j = 0; j < board->board_cells; j++)
        {
            board->directions[i][j] = compute_get_move_direction(board, i, j);
        }
    }

    board->reachable_cells = malloc(sizeof(directions_lines_t) * board->board_cells);
    assert(board->reachable_cells);
    for (size_t i = 0; i < board->board_cells; i++)
    {
        for (size_t dir = 0; dir < NUM_DIRS; dir++)
        {
            board->reachable_cells[i].dir_line[dir] = malloc(sizeof(unsigned int) * (board->board_width));
        }
        
        compute_queen_available_moves(board, &(board->reachable_cells[i]), i);
    }

    return board;
}

bool board_add_arrow(board_t *board, unsigned int index) {
    if (index > board->board_cells || board->arrows[index] == true)
        return false;
    board->arrows[index] = true;
    board->arrows_count++;
    return true;
}

void board_remove_arrow(board_t *board, unsigned int index){
    assert(board_get_index_state(board, index) == STATE_ARROW);
    board->arrows[index] = false;
    board->arrows_count--;
}


void board_free(board_t *board){
    destroy_graph(board->g);
    free(board->queens[0]);
    free(board->queens[1]);
    free(board->arrows);
    free(board->queen_occupy);
    for (size_t i = 0; i < board->board_cells; i++){
        free(board->directions[i]);
    }
    free(board->directions);

    for (size_t i = 0; i < board->board_cells; i++){
        for (size_t dir = 0; dir < NUM_DIRS; dir++)
        {
            free(board->reachable_cells[i].dir_line[dir]);
        }
        
    }
    free(board->reachable_cells);

    free(board);
}


unsigned int board_width(board_t *board){
    return board->board_width;
}

bool empty_board_index_is_available_from(board_t *board, unsigned int source, unsigned int dest){
    enum dir_t d = get_move_direction(board, source, dest);
    return empty_board_index_is_available(board, dest) && exist_edge_value(board->g, source, dest, d);
}

bool board_index_is_available_from(board_t *board, unsigned int source, unsigned int dest){
    enum dir_t d = get_move_direction(board, source, dest);
    return board_index_is_available(board, dest) && exist_edge_value(board->g, source, dest, d);
}


bool empty_board_index_is_available(board_t *board, unsigned int index){
    return (index < board->board_cells);
}

bool board_index_is_available(board_t *board, unsigned int index){
    return (index < board->board_cells) && (!board->arrows[index]) && (board->queen_occupy[index] == false);
}

enum cell_state board_get_index_state(board_t *board, unsigned int index){
    //If the index is greater than the grid max 
    if (index >= board->board_cells) return STATE_ERROR;
    if(board->arrows[index]) return STATE_ARROW;
    if(queens_occupy(board->queens[0], index, board->board_width)) return STATE_QUEEN_WHITE;
    if(queens_occupy(board->queens[1], index, board->board_width)) return STATE_QUEEN_BLACK;
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
            if (!cell_has_direct_neighbor(board, (i*width) + j)){
                printf("  ");
                continue;
            }
            printf("|");
            enum cell_state s = board_get_index_state(board, (i*width) + j);
            if (s == STATE_AVAILABLE) printf("_");
            if (s == STATE_ARROW) printf("X");
            if (s == STATE_QUEEN_WHITE) printf("♕");
            if (s == STATE_QUEEN_BLACK) printf("♛");
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

bool cell_has_direct_neighbor(board_t *board, unsigned int index){
    for (enum dir_t d = FIRST_DIR; d <= LAST_DIR; d++)
    {
        unsigned int dest = index + compute_step_toward_direction(d, board->board_width);
        if (dest > board->board_cells) continue;
        if (exist_edge(board->g, index, dest)){
            return true;
        }
    }
    
    return false;
}

void apply_move(board_t *board, struct move_t *move, unsigned int current_player){
    queens_move(board->queens[current_player], board->board_width, move->queen_src, move->queen_dst);
    board_add_arrow(board, move->arrow_dst);
    board->queen_occupy[move->queen_src] = false;
    board->queen_occupy[move->queen_dst] = true;
}

void undo_move(board_t *board, struct move_t *move, unsigned int current_player){
    board_remove_arrow(board, move->arrow_dst);
    queens_move(board->queens[current_player], board->board_width, move->queen_dst, move->queen_src);
    board->queen_occupy[move->queen_dst] = false;
    board->queen_occupy[move->queen_src] = true;
}

board_t * board_copy(board_t *board){
    board_t *copy = malloc(sizeof(*board));
    copy->g = graph_copy(board->g);
    copy->queens[0] = queens_copy(board->queens[0], board->board_width);
    copy->queens[1] = queens_copy(board->queens[1], board->board_width);
    copy->queens_count = board->queens_count;
    copy->board_width = (unsigned int) sqrt(copy->g->t->size1);
    copy->board_cells = board->board_width*board->board_width;
    copy->arrows_count = board->board_width*board->board_width;
    copy->arrows = malloc(sizeof(bool)*board->board_cells);
    for (unsigned int i = 0; i < board->board_cells; i++)
    {
        copy->arrows[i] = board->arrows[i];
    }
    copy->queen_occupy = malloc(sizeof(bool)*board->board_cells);
    for (unsigned int i = 0; i < board->board_cells; i++)
    {
        copy->queen_occupy[i] = board->queen_occupy[i];
    }

    copy->directions = malloc(sizeof(enum dir_t *) * board->board_cells);
    assert(copy->directions);

    for (size_t i = 0; i < board->board_cells; i++)
    {
        copy->directions[i] = malloc(sizeof(enum dir_t) * board->board_cells);
        assert(copy->directions[i]);
        for (size_t j = 0; j < board->board_cells; j++)
        {
            copy->directions[i][j] = board->directions[i][j];
        }
    }

    copy->reachable_cells = malloc(sizeof(directions_lines_t) * board->board_cells);
    assert(copy->reachable_cells);
    for (size_t i = 0; i < board->board_cells; i++)
    {
        for (size_t dir = 0; dir < NUM_DIRS; dir++)
        {
            copy->reachable_cells[i].dir_line[dir] = malloc(sizeof(unsigned int) * (board->board_width));
            for (size_t j = 0; j < board->board_width-1; j++)
            {
                copy->reachable_cells[i].dir_line[dir][j] = board->reachable_cells[i].dir_line[dir][j] ;
            }
            
        }
    }

    return copy;
}