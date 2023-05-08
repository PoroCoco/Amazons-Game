#include"territories.h"

bool index_can_improve(unsigned int index, unsigned int *territory, unsigned int board_width, unsigned int cells){
    int const steps[8] = {-1 - board_width, -board_width, 1 - board_width, 1, 1 + board_width, board_width, -1 + board_width, -1};

    for (unsigned int i = 0; i < 8; i++){
        if (index + steps[i] < cells && territory[index] < territory[index + steps[i]]) return true;
    }

    return false;
}

void breadth_fist_search_update(board_t* board, unsigned int* territory_array, unsigned int client, void get_moves(board_t *, queen_moves_t *, unsigned int)){
    struct queue* queue = queue_new(board->board_cells);
    
    struct queen_moves queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int) * board->board_cells * board->board_cells);
    assert(queen_moves.indexes);

    for(unsigned int i = 0; i < board->queens_count; i++){
        territory_array[board->queens[client][i]] = 0;
        queue_push(queue, board->queens[client][i]);
    }
    
    
    while(!queue_is_empty(queue)){
        unsigned int index = queue_pop(queue);
        
        if (!index_can_improve(index, territory_array, board->board_width, board->board_cells)) continue;
        
        get_moves(board, &queen_moves, index);
        while(queen_moves.move_count){
            unsigned int index_move = queen_moves.indexes[queen_moves.move_count - 1];
            if(territory_array[index_move] > territory_array[index] + 1 ){
                queue_push(queue, index_move);
                territory_array[index_move] = territory_array[index] + 1 ;
            }
            queen_moves.move_count--;

        }

    }
    queue_free(queue);
    free(queen_moves.indexes);
}

unsigned int* get_territory_queen_move(board_t* board, unsigned int client){
    unsigned int* territory_array = malloc(sizeof(unsigned int) * board->board_cells);
    for(unsigned int i = 0; i < board->board_cells; i++){
        territory_array[i] = UINT_MAX;
    }

    breadth_fist_search_update(board, territory_array, client, queen_available_moves);

    return territory_array;
}
unsigned int* get_territory_king_move(board_t* board, unsigned int client){
    unsigned int* territory_array = malloc(sizeof(unsigned int) * board->board_cells);
    for(unsigned int i = 0; i < board->board_cells; i++){
        territory_array[i] = UINT_MAX;
    }

    breadth_fist_search_update(board, territory_array, client, king_available_moves);

    return territory_array;
}