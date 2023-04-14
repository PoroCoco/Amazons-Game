#include "move_logic.h"

#include <assert.h>
#include "queens.h"

bool queen_can_move(board_t *board, unsigned int queen_board_index){
    for (enum dir_t d = FIRST_DIR; d <= LAST_DIR; d++)
    {
        int step = compute_step_toward_direction(d, board->board_width);
        if (board_index_is_available(board, queen_board_index + step)) return true;
    }

    return false;
}

bool is_game_won(board_t *board){
    for (size_t p = 0; p < NUM_PLAYERS; p++)
    {
        for (size_t i = 0; i < board->queens_count; i++)
        {
            if (queen_can_move(board, board->queens[p][i])) return false;
        }
    }
    
    return true;
}

enum dir_t get_move_direction(board_t *board, unsigned int origin, unsigned int destination)
{
    if (origin == destination) return DIR_ERROR;
    unsigned int width = board_width(board);
    unsigned int origin_x = origin % width;
    unsigned int origin_y = origin / width;

    unsigned int destination_x = destination % width;
    unsigned int destination_y = destination / width;

    //origin and destination on the same column
    if (origin_x == destination_x){
        return origin_y > destination_y ? DIR_NORTH : DIR_SOUTH;
    }

    //origin and destination on the same line
    if (origin_y == destination_y){
        return origin_x > destination_x ? DIR_WEST : DIR_EAST;
    }

    if(destination_x > origin_x){
        return destination_y > origin_y ? DIR_SE : DIR_NE;
    }

    if(destination_x < origin_x){
        return destination_y > origin_y ? DIR_SW : DIR_NW;
    }

    return DIR_ERROR;
}

int compute_step_toward_direction(enum dir_t direction, unsigned int board_width){
    int step = 0;

    switch (direction)
    {
    case DIR_NORTH:
        step = -board_width;
        break;
    
    case DIR_SOUTH:
        step = board_width;
        break;

    case DIR_EAST:
        step = 1;
        break;

    case DIR_WEST:
        step = -1;
        break;

    case DIR_NW:
        step = -1 - board_width;
        break;

    case DIR_NE:
        step = 1 - board_width;
        break;

    case DIR_SW:
        step = -1 + board_width;
        break;

    case DIR_SE:
        step = 1 + board_width;
        break;

    default:
        fprintf(stderr, "Impossible direction given to go from move source to move destination\n");
        exit(EXIT_FAILURE);
    }
    
    assert(step != 0);
    return step;
}

bool is_move_valid(board_t *board, struct move_t *move, unsigned int player_id)
{
    unsigned int current_position = move->queen_src;
    unsigned int destination = move->queen_dst;

    // check if there is a queen on the destination or source position
    for (unsigned int i = 0; i < NUM_PLAYERS; i++)
    {
        if (queens_occupy(board->queens[i], destination, board->board_width)) return false;

        if (!queens_occupy(board->queens[i], current_position, board->board_width) && (i == player_id)) return false;

        if (queens_occupy(board->queens[i], current_position, board->board_width) && (i != player_id)) return false;
    }
    

    int width = board_width(board);
    enum dir_t direction = get_move_direction(board, move->queen_src, move->queen_dst);
    if (direction == DIR_ERROR) return false;

    int step = compute_step_toward_direction(direction, width);


    while(current_position != destination){
        current_position += step;
        if(!board_index_is_available(board, current_position)) return false;
    }

    direction = get_move_direction(board, move->queen_dst, move->arrow_dst);
    if (direction == DIR_ERROR) return false;

    step = compute_step_toward_direction(direction, width);

    current_position = move->queen_dst;
    destination = move->arrow_dst;
    while(current_position != destination){
        current_position += step;
        if(!board_index_is_available(board, current_position)) return false;
    }



    return true;
}