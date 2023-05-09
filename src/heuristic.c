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

double territory_heuristic_average(board_t *board, unsigned int player_id,  struct territory_cell* get_array(board_t *, unsigned int, struct queue *, struct queen_moves, struct territory_cell*), struct queue * queue, struct queen_moves queen_moves_territory, struct territory_cell* territory_array_player, struct territory_cell* territory_array_ennemy){
    unsigned int ennemy_id = get_ennemy_id(player_id);
    territory_array_ennemy = get_array(board,ennemy_id, queue, queen_moves_territory, territory_array_player);
    territory_array_player =  get_array(board, player_id, queue, queen_moves_territory, territory_array_ennemy);
    double sum_player = 0;
    unsigned int tile_count_player = 0;
    for(unsigned int i = 0; i < board->board_cells; i++){
        tile_count_player++;
        sum_player+= delta(territory_array_player [i].distance, territory_array_ennemy[i].distance);
    }
    return sum_player;
}



