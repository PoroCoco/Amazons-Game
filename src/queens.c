#include <stdlib.h>

#include "queens.h"
#include <assert.h>

unsigned int queens_compute_number(unsigned int board_width){
    assert(board_width >= 5);
    return 4*(board_width/10 + 1);
}

unsigned int* queens_create_positions(unsigned int board_width, unsigned int player_id){
    assert(player_id == 1 || player_id == 0);

    unsigned int queens_number = queens_compute_number(board_width);
    unsigned int* queens_positions = malloc(sizeof(unsigned int) * queens_number);
    
    unsigned int separing_gap = ((board_width - (queens_number) - 1) / (queens_number/2)) + 1; 

    unsigned int delta_horizontal = board_width * (board_width - 1) * player_id;

    for(unsigned int index = 0; index < (queens_number / 4); index++){   
        queens_positions[index] = separing_gap + index * separing_gap + index;
        queens_positions[queens_number / 2 - index - 1] = board_width - 1 - queens_positions[index]; 
        queens_positions[index] += delta_horizontal;
        queens_positions[queens_number / 2 - index - 1] += delta_horizontal; //same for the symetric
    }

    for(unsigned int index = queens_number / 2, cpt = 0; index < queens_number - 1; index = index + 2, cpt++){        
        queens_positions[index] = (separing_gap + cpt * separing_gap + cpt ) * board_width;
        queens_positions[index + 1] = queens_positions[index] + board_width - 1;
        if(player_id){
            queens_positions[index] = (board_width * board_width) - queens_positions[index] -1;
            queens_positions[index + 1] = (board_width * board_width) - queens_positions[index + 1] -1;
        }

    }
    return queens_positions;
}

void queens_free_positions(unsigned int *queens){
    assert(queens);
    free(queens);
}

unsigned int* queens_copy(const unsigned int *queens,  unsigned int board_width){
    assert(queens);
    unsigned int queens_count = 4*(board_width/10 + 1);
    unsigned int *queens_cpy = malloc(sizeof(*queens_cpy) * queens_count);
    for (unsigned int i = 0; i < queens_count; i++){
        queens_cpy[i] = queens[i];
    }
    return queens_cpy;
}

bool queens_occupy(const unsigned int *queens, unsigned int index, unsigned int board_width){
    assert(queens != NULL);
    unsigned int queens_count = 4*(board_width/10 + 1);
    for (unsigned int i = 0; i < queens_count; i++){
        if(queens[i] == index) return true;    
    }
    return false;
}


void queens_move(unsigned int *queens, unsigned int board_width, unsigned int src, unsigned int dst){
    assert(queens);
    unsigned int queens_count = 4*(board_width/10 + 1);
    assert(dst < (board_width * board_width));
    for (unsigned int i = 0; i < queens_count; i++){
        if(queens[i] == src){
            queens[i] = dst;
            return;
        }
    }
    assert(NULL);
}