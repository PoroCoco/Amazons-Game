#include <stdlib.h>

#include "queens.h"
#include <assert.h>

unsigned int queens_compute_number(unsigned int board_size){
    assert(board_size >= 5);
    return 4*(board_size/10 + 1);
}

unsigned int* queens_create_positions(unsigned int board_size, unsigned int player_id){
    unsigned int queens_number = queens_compute_number(board_size);
    unsigned int* queens_positions = malloc(sizeof(unsigned int) * queens_number);

    unsigned int separing_gap = ((board_size - 2 * queens_number - 1) / queens_number) + 1; 

    for(unsigned int index = 0; index < (queens_number / 4); index++){   
        queens_positions[index] = separing_gap + index * separing_gap;
        queens_positions[queens_number / 2 - index - 1] = board_size - 1 - queens_positions[index]; //same for the symetric
    }

    for(unsigned int index = queens_number / 2; index < 3 * (queens_number / 4); index = index + 2){        
        queens_positions[index] = (separing_gap + index * separing_gap) * board_size ;
        queens_positions[index + 1] = queens_positions[index] + board_size - 1 + 10;
    }

    return queens_positions;
}

void queens_free_positions(unsigned int *queens){
    assert(queens);
    free(queens);
}

unsigned int* queens_copy(const unsigned int *queens, unsigned int queen_count){
    unsigned int *queens_cpy = malloc(sizeof(*queens_cpy)*queen_count);
    for (unsigned int i = 0; i < queen_count; i++)
    {
        queens_cpy[i] = queens[i];
    }
    
    return queens_cpy;
}

bool queens_occupy(const unsigned int *queens, unsigned int index, unsigned int board_width){
    assert(queens != NULL);
    unsigned int queens_count = queens_compute_number(board_width);
    for (unsigned int i = 0; i < queens_count; i++){
        if(queens[i] == index) return true;    
    }
    return false;
}
