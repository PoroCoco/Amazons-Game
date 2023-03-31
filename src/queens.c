#include <stdlib.h>

#include "queens.h"

unsigned int queens_compute_number(unsigned int board_size){
    return 4*(board_size/10 + 1);
}

unsigned int* queens_create_positions(unsigned int queens_number, unsigned int player_id){
    return NULL;
}

unsigned int* queens_copy(const unsigned int *queens, unsigned int queens_number){
    return NULL;
}

bool queens_occupy(const unsigned int *queens, unsigned int index, unsigned int board_size){
    unsigned int queens_count = queens_compute_number(board_size);
    for (unsigned int i = 0; i < queens_count; i++)
        if(queens[i] == index) return true;    
    return false;
}