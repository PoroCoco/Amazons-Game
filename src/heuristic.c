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
                board_powers[stack[stack_count - 1]] += direction_cells - stack_count + 1;
            }
        }
    }

    unsigned int *board_powers_player = calloc(board->board_cells, sizeof(*board_powers)); 

    for (unsigned int i = 0; i < board->queens_count; i++)
    {
        unsigned int current_queen_index = board->queens[player_id][i];
        for (enum dir_t d = FIRST_DIR; d <= LAST_DIR; d++)
        {
            stack_count = 0;
            unsigned int current_index = current_queen_index;
            unsigned int previous_index = current_queen_index;
            int step = compute_step_toward_direction(d, board->board_width);
            current_index += step;

            while(current_index < board->board_cells){
                if (!board_index_is_available_from(board, previous_index, current_index)) break;
                stack[stack_count] = current_index;
                stack_count++;
                previous_index = current_index;
                current_index += step;
            }
            
            unsigned int direction_cells = stack_count;
            for (; stack_count > 0; stack_count--)
            {
                board_powers_player[stack[stack_count - 1]] += direction_cells - stack_count + 1;
            }
        }
    }
    
    for (size_t i = 0; i < board->board_cells; i++)
    {
        current_heuristic += board_powers[i] - (COEFF_SAFE*board_powers_player[i]);

    }


    free(stack);
    free(board_powers);
    free(board_powers_player);
    return 1./ (current_heuristic+1.);
}

double power_heuristic(board_t *board, unsigned int player_id){
    
    double current_heuristic = 0;

    unsigned int *board_powers = calloc(board->board_cells, sizeof(*board_powers)); 

    unsigned int *stack = malloc(sizeof(*stack) * (board->board_width/2) * 6); 
    unsigned int stack_count = 0;

    //for every ennemy queen
    for (unsigned int i = 0; i < board->queens_count; i++)
    {
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
                board_powers[stack[stack_count - 1]] += direction_cells - stack_count + 1;
            }
        }
    }
    
    for (size_t i = 0; i < board->board_cells; i++)
    {
        current_heuristic += board_powers[i];
    }



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
    //returns 0 if the tile is not reachable for both
    if(val1 == UINT_MAX && val2== UINT_MAX){
        return 0;
    }
    //returns 0 if there is a queen in the tile
    if(val1 == 0 || val2== 0){
        return 0;
    }
    //the tile is accesible to the ennemy but not to me :(
    else if(val1 == UINT_MAX){
        return -3;
    }
    //the tile is accessible to me but not to the ennemy :)
    else if(val2 == UINT_MAX){
        return 3;
    }
    //same distane to the tile
    if(val1 == val2){
        return 0;
    }
    //more accessible to me
    else if(val1 < val2){
        return val2 - val1;
    }
    //more accessible to him
    else{
        return -1;
    }
}

double territory_heuristic_average(board_t *board, unsigned int player_id){
    unsigned int ennemy_id = get_ennemy_id(player_id);
    struct territory_cell* territory_ennemy = get_territory_queen_move(board,ennemy_id);
    struct territory_cell* territory_player =  get_territory_queen_move(board, player_id);
    double sum_player = 0;
    unsigned int tile_count_player = 0;
    for(unsigned int i = 0; i < board->board_cells; i++){
        tile_count_player++;
        sum_player+= delta(territory_player [i].distance, territory_ennemy[i].distance);
    }
    free(territory_ennemy);
    free(territory_player);
    return sum_player;
}



double variance(unsigned int arr[], int n) {
    double mean = 0.0, variance = 0.0;
    int i;

    for (i = 0; i < n; i++) {
        mean += arr[i];
    }
    mean /= n;
    for (i = 0; i < n; i++) {
        variance += (arr[i] - mean) * (arr[i] - mean);
    }
    variance /= n;

    return variance;
}

double kings_delta(unsigned int val1,unsigned int val2){
    if(val1 == UINT_MAX || val2== UINT_MAX){
        return 0;
    }
    if(val1 == 0 || val2== 0){
        return 0;
    }

    float result =  val2 - val1 / 2;
    result = (result > -1) ? result : -1;
    result = (result < 1) ? result : 1;
    return result;
}

double improved_delta(unsigned int val1,unsigned int val2){
    if(val1 == UINT_MAX && val2== UINT_MAX){
        return 0;
    }
    if(val1 == 0 || val2== 0){
        return 0;
    }
    else if(val1 == UINT_MAX){
        return -1/pow(2,val2);
    }
    else if(val2 == UINT_MAX){
        return 1/pow(2,val1);
    }
    return 1/pow(2,val1) - 1/pow(2,val2);
}

 struct heuristic_data automatic_territory(board_t *board, unsigned int player_id){
    unsigned int ennemy_id = get_ennemy_id(player_id);
    struct territory_cell* territory_ennemy = get_territory_queen_move(board,ennemy_id);
    struct territory_cell* territory_player =  get_territory_queen_move(board, player_id);
    struct territory_cell* territory_ennemy_king = get_territory_king_move(board,ennemy_id);
    struct territory_cell* territory_player_king =  get_territory_king_move(board, player_id);
    double sum_player = 0;
    double sum_king = 0;
    unsigned int* queens_access = malloc (sizeof(unsigned int) * board->queens_count);
    for(unsigned int i = 0; i < board->queens_count; i++){
        queens_access[i] = 0; 
    }
    for(unsigned int i = 0; i < board->board_cells; i++){
        sum_player+= improved_delta(territory_player[i].distance, territory_ennemy[i].distance);
    }
    for(unsigned int i = 0; i < board->board_cells; i++){
        sum_king+= kings_delta(territory_player_king[i].distance, territory_ennemy_king[i].distance);
        if(territory_player[i].queens_index != UINT_MAX){
            queens_access[territory_player[i].queens_index]++; 
        }
    }


    free(territory_ennemy);
    free(territory_player);
    free(territory_ennemy_king);
    free(territory_player_king);
    double var = variance(queens_access,board->queens_count);
    free(queens_access);

    //printf("king %f queen %f",sum_king,sum_player);
    sum_king = (sum_king/(double)(5 +  ((board->arrows_count) / (board->board_width))));
    struct heuristic_data result = {
        .variance = var,
        .heuristic = sum_player + sum_king
    };
    return result;
}

 struct heuristic_data automatic_explosion(board_t *board, unsigned int player_id){
    unsigned int ennemy_id = get_ennemy_id(player_id);
    struct territory_cell* territory_ennemy = get_territory_queen_move(board,ennemy_id);
    struct territory_cell* territory_player =  get_territory_queen_move(board, player_id);
    double sum_player = 0;
    unsigned int tile_count_player = 0;
    unsigned int* queens_access = malloc (sizeof(unsigned int) * board->queens_count);
    for(unsigned int i = 0; i < board->queens_count; i++){
            queens_access[i] = 0; 
    }

    for(unsigned int i = 0; i < board->board_cells; i++){
        tile_count_player++;
        sum_player+= improved_delta(territory_player[i].distance, territory_ennemy[i].distance);

        if(territory_player[i].queens_index != UINT_MAX){
            queens_access[territory_player[i].queens_index]++; 
        }
    }
    free(territory_ennemy);
    free(territory_player);
    double var = variance(queens_access,board->queens_count);
    free(queens_access);
    struct heuristic_data result = {
        .variance = var,
        .heuristic = sum_player
    };
    return result;
}
