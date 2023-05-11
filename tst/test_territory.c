#include"queue.h"
#include "board.h"
#include "graph_ext.h"
#include "queens.h"
#include "territories.h"
#include "limits.h"



int test_queue(void){
    struct queue* queue = queue_new(100);
    for(unsigned int i = 0; i < 22; i++){
        queue_push(queue, i);
    }
    for(unsigned int i = 0; i < 22; i++){
        if(queue_pop(queue) != i){
            return 1;
        }
    }
    for(unsigned int i = 0; i < 77; i++){
        queue_push(queue, i);
        if(queue_pop(queue) != i){
            return 1;
        }
    }
    queue_free(queue);
    return 0;
}

int test_territory_array(void){
    
    unsigned int board_size = 10;
    unsigned int queen_number = queens_compute_number(board_size)/2;
    unsigned int *queens[NUM_PLAYERS]; 

    unsigned int q0[4] = {3, 24, 83, 86};
    unsigned int q1[4] = {12, 36, 63, 77}; 
    queens[0] = q0;
    queens[1] = q1;
    struct graph_t *g = create_graph(board_size, convert_char_to_shape('s'));
    board_t *game_board = board_create(g, queens, queen_number);
    unsigned int shooted_arrows[26]={2, 4, 13, 14, 22, 23, 25, 26, 27, 28, 
    31, 32, 34, 38, 40, 45, 51, 52, 55, 56, 57, 67, 71, 79, 90, 97};
    for(int i = 0; i < 26; i++){
        board_add_arrow(game_board,shooted_arrows[i]);
    }
    struct territory_cell* computed_array_queens_p0 = get_territory_queen_move(game_board, 0);
    struct territory_cell* computed_array_queens_p1 = get_territory_queen_move(game_board, 1);

    unsigned int expected_array_queens_p0[100] = {
        4,5,UINT_MAX,0,UINT_MAX,2,1,2,2,2,
        4,5,UINT_MAX,UINT_MAX,UINT_MAX,1,2,2,2,2,
        4,4,UINT_MAX,UINT_MAX,0,UINT_MAX,UINT_MAX,UINT_MAX,UINT_MAX,3,
        3,UINT_MAX,UINT_MAX,1,UINT_MAX,1,UINT_MAX,2,UINT_MAX,3,
        UINT_MAX,2,1,2,2,UINT_MAX,1,2,2,2,
        1,UINT_MAX,UINT_MAX,1,2,UINT_MAX,UINT_MAX,UINT_MAX,2,3,
        2,1,2,UINT_MAX,1,1,1,UINT_MAX,2,2,
        2,UINT_MAX,1,1,1,1,1,UINT_MAX,2,UINT_MAX,
        1,1,1,0,1,1,0,1,1,1,
        UINT_MAX,2,1,1,1,1,1,UINT_MAX,2,2,
    };
    unsigned int expected_array_queens_p1[100] = {
        2,1,UINT_MAX,UINT_MAX,UINT_MAX,3,3,3,3,2,
        1,1,0,UINT_MAX,UINT_MAX,3,3,3,3,2,
        2,1,UINT_MAX,UINT_MAX,UINT_MAX,UINT_MAX,UINT_MAX,UINT_MAX,UINT_MAX,2,
        1,UINT_MAX,UINT_MAX,1,UINT_MAX,1,0,1,UINT_MAX,2,
        UINT_MAX,2,2,1,2,UINT_MAX,1,1,2,2,
        2,UINT_MAX,UINT_MAX,1,1,UINT_MAX,UINT_MAX,UINT_MAX,1,1,
        1,1,1,0,1,1,1,UINT_MAX,1,1,
        2,UINT_MAX,1,1,1,1,1,0,1,UINT_MAX,
        2,1,2,UINT_MAX,2,1,UINT_MAX,1,1,2,
        UINT_MAX,2,2,2,2,2,1,UINT_MAX,2,1,
    };




    struct territory_cell* computed_array_king_p0 = get_territory_king_move(game_board, 0);
    struct territory_cell* computed_array_king_p1 = get_territory_king_move(game_board, 1);
    unsigned int expected_array_king_p0[100] = {
        7,7,UINT_MAX,0,UINT_MAX,2,2,3,4,5,
        6,6,UINT_MAX,UINT_MAX,UINT_MAX,1,2,3,4,5,
        5,5,UINT_MAX,UINT_MAX,0,UINT_MAX,UINT_MAX,UINT_MAX,UINT_MAX,5,
        4,UINT_MAX,UINT_MAX,1,UINT_MAX,1,UINT_MAX,3,UINT_MAX,5,
           UINT_MAX,3,2,2,2,UINT_MAX,2,3,4,5,
        3,UINT_MAX,UINT_MAX,3,3,UINT_MAX,UINT_MAX,UINT_MAX,4,4,
        3,2,2,UINT_MAX,2,2,2,UINT_MAX,3,3,
        3,UINT_MAX,1,1,1,1,1,UINT_MAX,2,UINT_MAX,
        3,2,1,0,1,1,0,1,2,3,
        UINT_MAX,2,1,1,1,1,1,UINT_MAX,2,3,
    };
    unsigned int expected_array_king_p1[100] = {
        2,1,UINT_MAX,UINT_MAX,UINT_MAX,8,7,6,6,6,
        2,1,0,UINT_MAX,UINT_MAX,8,7,6,5,5,
        2,1,UINT_MAX,UINT_MAX,UINT_MAX,UINT_MAX,UINT_MAX,UINT_MAX,UINT_MAX,4,
        2,UINT_MAX,UINT_MAX,3,UINT_MAX,1,0,1,UINT_MAX,3,
        UINT_MAX,3,2,2,2,UINT_MAX,1,1,2,3,
        3,UINT_MAX,UINT_MAX,1,1,UINT_MAX,UINT_MAX,UINT_MAX,2,2,
        3,2,1,0,1,2,1,UINT_MAX,1,2,
        3,UINT_MAX,1,1,1,2,1,0,1,UINT_MAX,
        3,2,2,UINT_MAX,2,2,UINT_MAX,1,1,2,
        UINT_MAX,3,3,3,3,3,2,UINT_MAX,2,2,
    };


    for(int i = 0; i < 100; i++){
        if(expected_array_queens_p1[i] != computed_array_queens_p1[i].distance || expected_array_queens_p0[i] != computed_array_queens_p0[i].distance || 
        expected_array_king_p1[i] != computed_array_king_p1[i].distance || expected_array_king_p0[i] != computed_array_king_p0[i].distance){
            return !0;
        }
    }
/*
    unsigned int* queens_access = malloc (sizeof(unsigned int) * 4 );
    for(unsigned int i = 0; i < 4; i++){
            queens_access[i] = 0; 
    }
    for(unsigned int i = 0; i < 100; i++){
        if(computed_array_queens_p1[i].queens_index > 4){
            printf("| ");
        }
        else{
        printf("%d ",computed_array_queens_p1[i].queens_index);}
        if(i % 10 == 9){
            printf("\n");
        }
        if(computed_array_queens_p1[i].queens_index != UINT_MAX){
            queens_access[computed_array_queens_p1[i].queens_index]++; 
        }
    }
    printf("%f\n", variance(queens_access,4));*/
    free(computed_array_queens_p0);
    free(computed_array_queens_p1);
    free(computed_array_king_p0);
    free(computed_array_king_p1);    
    destroy_graph(game_board->g);
    
    free(game_board->arrows);
    free(game_board->queen_occupy);
    for (size_t i = 0; i < game_board->board_cells; i++){
        free(game_board->directions[i]);
    }
    free(game_board->directions);

    for (size_t i = 0; i < game_board->board_cells; i++){
        for (size_t dir = 0; dir < NUM_DIRS; dir++)
        {
            free(game_board->reachable_cells[i].dir_line[dir]);
        }
        
    }
    free(game_board->reachable_cells);
    free(game_board);
    return 0;
}