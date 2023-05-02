#include "heuristic.h"

#define COEFF_SAFE 0.1

double power_heuristic_safe(board_t *board, unsigned int player_id){
    
    double current_heuristic = 0;

    unsigned int *board_powers = calloc(board->board_cells, sizeof(*board_powers)); 

    unsigned int *stack = malloc(sizeof(*stack) * (board->board_width/2) * 6); 
    unsigned int stack_count = 0;

    //for every ennemy queen
    for (unsigned int i = 0; i < board->queens_count; i++)
    {
        // printf("heuristic for queen at %u\n", board->queens[1 - player_id][i]);
        unsigned int current_queen_index = board->queens[1 - player_id][i];
        for (enum dir_t d = FIRST_DIR; d <= LAST_DIR; d++)
        {
            stack_count = 0;
            unsigned int current_index = current_queen_index;
            unsigned int previous_index = current_queen_index;
            int step = compute_step_toward_direction(d, board->board_width);
            current_index += step;

            //adding to the stack each valid cell in the current direction
            while(current_index < board->board_cells){
                if (!board_index_is_available_from(board, previous_index, current_index)) break;
                stack[stack_count] = current_index;
                stack_count++;
                previous_index = current_index;
                current_index += step;
            }
            
            //popping ths stack to add the value to each cell
            unsigned int direction_cells = stack_count;
            for (; stack_count > 0; stack_count--)
            {
                // printf("poping %u with value %u\n",stack[stack_count - 1], direction_cells - stack_count + 1);
                board_powers[stack[stack_count - 1]] += direction_cells - stack_count + 1;
            }
        }
    }

    unsigned int *board_powers_player = calloc(board->board_cells, sizeof(*board_powers)); 

    for (unsigned int i = 0; i < board->queens_count; i++)
    {
        // printf("heuristic for queen at %u\n", board->queens[1 - player_id][i]);
        unsigned int current_queen_index = board->queens[player_id][i];
        for (enum dir_t d = FIRST_DIR; d <= LAST_DIR; d++)
        {
            stack_count = 0;
            unsigned int current_index = current_queen_index;
            unsigned int previous_index = current_queen_index;
            int step = compute_step_toward_direction(d, board->board_width);
            current_index += step;

            //adding to the stack each valid cell in the current direction
            while(current_index < board->board_cells){
                if (!board_index_is_available_from(board, previous_index, current_index)) break;
                stack[stack_count] = current_index;
                stack_count++;
                previous_index = current_index;
                current_index += step;
            }
            
            //popping ths stack to add the value to each cell
            unsigned int direction_cells = stack_count;
            for (; stack_count > 0; stack_count--)
            {
                // printf("poping %u with value %u\n",stack[stack_count - 1], direction_cells - stack_count + 1);
                board_powers_player[stack[stack_count - 1]] += direction_cells - stack_count + 1;
            }
        }
    }
    
    for (size_t i = 0; i < board->board_cells; i++)
    {
        current_heuristic += board_powers[i] - (COEFF_SAFE*board_powers_player[i]);
        // if(i%board->board_width == 0) printf("\n");
        // printf("%u ", board_powers[i]);
    }
    // printf("\n");


    free(stack);
    free(board_powers);
    free(board_powers_player);
    return 1./ (current_heuristic+1.);
}



#include "heuristic.h"


double power_heuristic(board_t *board, unsigned int player_id){
    
    double current_heuristic = 0;

    unsigned int *board_powers = calloc(board->board_cells, sizeof(*board_powers)); 

    unsigned int *stack = malloc(sizeof(*stack) * (board->board_width/2) * 6); 
    unsigned int stack_count = 0;

    //for every ennemy queen
    for (unsigned int i = 0; i < board->queens_count; i++)
    {
        // printf("heuristic for queen at %u\n", board->queens[1 - player_id][i]);
        unsigned int current_queen_index = board->queens[1 - player_id][i];
        for (enum dir_t d = FIRST_DIR; d <= LAST_DIR; d++)
        {
            stack_count = 0;
            unsigned int current_index = current_queen_index;
            unsigned int previous_index = current_queen_index;
            int step = compute_step_toward_direction(d, board->board_width);
            current_index += step;

            //adding to the stack each valid cell in the current direction
            while(current_index < board->board_cells){
                if (!board_index_is_available_from(board, previous_index, current_index)) break;
                stack[stack_count] = current_index;
                stack_count++;
                previous_index = current_index;
                current_index += step;
            }
            
            //popping ths stack to add the value to each cell
            unsigned int direction_cells = stack_count;
            for (; stack_count > 0; stack_count--)
            {
                // printf("poping %u with value %u\n",stack[stack_count - 1], direction_cells - stack_count + 1);
                board_powers[stack[stack_count - 1]] += direction_cells - stack_count + 1;
            }
        }
    }
    
    for (size_t i = 0; i < board->board_cells; i++)
    {
        current_heuristic += board_powers[i];
        // if(i%board->board_width == 0) printf("\n");
        // printf("%u ", board_powers[i]);
    }
    // printf("\n");


    free(stack);
    free(board_powers);
    return 1./ (current_heuristic+1.);
}




unsigned int get_ennemy_id (unsigned int player_id){
    if(player_id)
        return 0;
    else    
        return 1;
}   



double delta(unsigned int val1,unsigned int val2){
    if(val1 == UINT_MAX && val2== UINT_MAX){
        return 0;
    }
    if(val1 == 0 || val2== 0){
        return 0;
    }
    else if(val1 == UINT_MAX){
        return -3;
    }
    else if(val2 == UINT_MAX){
        return 3;
    }
    if(val1 == val2){
        return 0;
    }
    else if(val1 < val2){
        return val2 - val1;
    }
    else{
        return -1;
    }
}

double territory_heuristic_average(board_t *board, unsigned int player_id,  unsigned int* get_array(board_t *, unsigned int)){
    unsigned int ennemy_id = get_ennemy_id(player_id);
    unsigned int* territory_ennemy = get_array(board,ennemy_id);
    unsigned int* territory_player =  get_array(board, player_id);
    double sum_player = 0, sum_ennemy = 0;
    unsigned int tile_count_player = 0, tile_count_ennemy = 0;
    for(unsigned int i = 0; i < board->board_cells; i++){
        //if(territory_player[i] && territory_player[i] != UINT_MAX){
            tile_count_player++;
            sum_player+= delta(territory_player[i], territory_ennemy[i]);
        //}
        /*
        if(territory_ennemy[i] && territory_ennemy[i] != UINT_MAX){
            tile_count_ennemy++;
            sum_ennemy += territory_ennemy[i];
        }*/
    }
    // (tile_count_player*1.0 + 1.0)
    if(!sum_player){
        return 0;
    }
/*
    printf("Player:\n");
    for(unsigned int i = 0; i <  board->board_cells; i++){
        if(territory_ennemy[i] > 10){
            printf("| ");
        }
        else{
            printf("%d ",territory_ennemy[i]);
        }
        if(i%6 == 5){
            printf("\n");
        }
    }
    printf("\n");


    printf("Ennemy:\n");
    for(unsigned int i = 0; i <  board->board_cells; i++){
        if(territory_player[i] > 10){
            printf("| ");
        }
        else{
            printf("%d ",territory_player[i]);
        }
        if(i%6 == 5){
            printf("\n");
        }
    }
    printf("\n");
    printf("%f\n",sum_player);
*/

    return sum_player;
}



