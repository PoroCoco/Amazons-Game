#include"territories.h"

bool index_can_improve(unsigned int index, struct territory_cell *territory, unsigned int board_width, unsigned int cells){
    int const steps[8] = {-1 - board_width, -board_width, 1 - board_width, 1, 1 + board_width, board_width, -1 + board_width, -1};

    for (unsigned int i = 0; i < 8; i++){
        if (index + steps[i] < cells && territory[index].distance < territory[index + steps[i]].distance) return true;
    }

    return false;
}

void breadth_fist_search_update(board_t* board, struct territory_cell* territory_array, unsigned int client, void get_moves(board_t *, queen_moves_t *, unsigned int)){
    struct queue* queue = queue_new(board->board_cells);
    
    struct queen_moves queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int) * board->board_cells * board->board_cells);
    assert(queen_moves.indexes);

    for(unsigned int i = 0; i < board->queens_count; i++){
        territory_array[board->queens[client][i]].distance = 0;
        territory_array[board->queens[client][i]].queens_index = i;
        queue_push(queue, board->queens[client][i]);
    }
    
    while(!queue_is_empty(queue)){
        unsigned int index = queue_pop(queue);
        if (!index_can_improve(index, territory_array, board->board_width, board->board_cells)) continue;       
        get_moves(board, &queen_moves, index);
        while(queen_moves.move_count){
            unsigned int index_move = queen_moves.indexes[queen_moves.move_count - 1];
            if(territory_array[index_move].distance > territory_array[index].distance + 1 ){
                queue_push(queue, index_move);
                territory_array[index_move].distance = territory_array[index].distance + 1 ;
                territory_array[index_move].queens_index = territory_array[index].queens_index;
            }
            queen_moves.move_count--;
        }
    }
    queue_free(queue);
    free(queen_moves.indexes);
}

struct territory_cell* get_territory_queen_move(board_t* board, unsigned int client){
    struct territory_cell* territory_array = malloc(sizeof(struct territory_cell) * board->board_cells);
    for(unsigned int i = 0; i < board->board_cells; i++){
        territory_array[i].distance = UINT_MAX;
        territory_array[i].queens_index = UINT_MAX;       
    }

    breadth_fist_search_update(board, territory_array, client, queen_available_moves);

    return territory_array;
}
struct territory_cell* get_territory_king_move(board_t* board, unsigned int client){
    struct territory_cell* territory_array = malloc(sizeof(struct territory_cell) * board->board_cells);
    for(unsigned int i = 0; i < board->board_cells; i++){
        territory_array[i].distance = UINT_MAX;
        territory_array[i].queens_index = UINT_MAX;
    }

    breadth_fist_search_update(board, territory_array, client, king_available_moves);

    return territory_array;
}