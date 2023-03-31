#include "client_random.h"
#include "board.h"
#include <math.h>

struct random_client *c = NULL;

enum dir_t get_move_direction(random_client_t *client, unsigned int origin, unsigned int destination)
{
    unsigned int width = board_width(client->board);
    if (origin == destination) return DIR_ERROR;

    if (abs(origin - destination) < width){
        return origin - destination > 0 ? DIR_EAST : DIR_WEST;
    }

    if ((destination % width) == (origin % width)){
        return origin - destination > 0 ? DIR_NORTH : DIR_SOUTH;
    }

    return DIR_ERROR;
}

bool is_move_valid(struct random_client *c, struct move_t *move)
{
    int width = board_width(c->board);
    enum dir_t direction = get_move_direction(move->queen_src, move->queen_dst, width);
    if (direction == DIR_ERROR) return false;

    unsigned int current_position = move->queen_src;
    unsigned int destination = move->queen_dst;

    int step = 0;
    switch (direction)
    {
    case DIR_NORTH:
        step = -width;
        break;
    
    case DIR_SOUTH:
        step = width;
        break;

    case DIR_EAST:
        step = 1;
        break;

    case DIR_WEST:
        step = -1;
        break;

    default:
        fprintf(stderr, "Impossible direction given to go from move source to move destination\n");
        exit(EXIT_FAILURE);
        break;
    }

    while(current_position != destination){
        current_position += step;
        if(!board_index_is_available(c->board, current_position)) return false;
    }

    // check is there is a queen on the destination edge
    if (queens_occupy(c->board->queens[0], move->queen_dst, c->board->board_width)) return false;
    if (queens_occupy(c->board->queens[1], move->queen_dst, c->board->board_width)) return false;

    // check is there is a queen on the source edge
    if (queens_occupy(c->board->queens[0], move->queen_src, c->board->board_width)) return false;
    if (queens_occupy(c->board->queens[1], move->queen_src, c->board->board_width)) return false;

    return true;
}

char const *get_player_name(void)
{
    return c->name;
}

void initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
    if (c == NULL)
    {
        c = malloc(sizeof(struct random_client));
        c->name = "Random";
        c->id = player_id;
        c->board = board_create(graph, queens, num_queens);
    }
}

struct move_t play(struct move_t previous_move)
{
    return previous_move;
}

void finalize()
{
    printf("finalize for me client id %u, my ptr is %p\n", c->id, c);
    // destroy_graph(c->graph);
    // free(c);
}