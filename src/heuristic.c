#include "heuristic.h"


double power_heuristic(board_t *board, unsigned int player_id){
    
    unsigned int current_heuristic = 0;

    unsigned int *board_powers = calloc(board->board_cells, sizeof(*board_powers)); 

    unsigned int *stack = malloc(sizeof(*stack) * (board->board_width/2) * 6); 
    unsigned int stack_count = 0;
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




