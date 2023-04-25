#include"territories.h"

void breadth_fist_search_update(board_t* board, unsigned int* territory_array, unsigned int queen_index){
    struct queue* queue = queue_new(board->board_cells);
    
    struct queen_moves queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int) * board->board_cells * board->board_cells);
    assert(queen_moves.indexes);

    unsigned int distance_from_queen = 0;

    territory_array[queen_index] = distance_from_queen;
    distance_from_queen++;
    queue_push(queue, queen_index);
    
    while(!queue_is_empty(queue)){
        unsigned int index = queue_pop(queue);
        queen_available_moves(board, &queen_moves, index);
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

unsigned int* get_territory_array(board_t* board, unsigned int client){
    unsigned int* territory_array = malloc(sizeof(unsigned int) * board->board_cells);
    for(unsigned int i = 0; i < board->board_cells; i++){
        territory_array[i] = UINT_MAX;
    }
    for(unsigned int i = 0; i < board->queens_count; i++)
        breadth_fist_search_update(board, territory_array, board->queens[client][i]);

    return territory_array;
}