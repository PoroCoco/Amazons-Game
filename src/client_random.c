#include "client_random.h"
#include "stdbool.h"

struct random_client *c = NULL;

enum dir_t get_move_direction(size_t e1, size_t e2, size_t width)
{
    int origin = (int)e1;
    int destination = (int)e2;
    if (origin == destination || !origin || !destination || !width)
        return -1;

    if (abs(origin - destination) < width)
    {

        return origin - destination > 0 ? DIR_EAST : DIR_WEST;
    }
    else
    {
        if ((destination % width) == (origin % width))
        {
            return origin - destination > 0 ? DIR_NORTH : DIR_SOUTH;
        }

        return 9;
    }
}

int is_move_valid(struct random_client *c, struct move_t *move)
{

    int width = board_width(c->graph);
    int direction = get_move_direction(move->queen_src, move->queen_dst, width);
    int current_position = (int)move->queen_src;
    int destination = (int)move->queen_dst;
    int intermediate_position = (int)move->queen_src;

    switch (direction)
    {
    case DIR_EAST:
        intermediate_position++;
        break;
    case DIR_WEST:
        intermediate_position--;
        break;
    case DIR_NORTH:
        intermediate_position += width;
        break;
    case DIR_SOUTH:
        intermediate_position -= width;
        break;

    default:
        current_position++;
        break;
    }

    // check if destination edge exist
    while (current_position != destination && intermediate_position < 0)
    {

        if (intermediate_position < 0 ||
            intermediate_position > width ||
            !exist_edge(c->graph, current_position, intermediate_position))
            return false;

        switch (direction)
        {
        case DIR_EAST:
            current_position++;
            intermediate_position++;
            break;
        case DIR_WEST:
            current_position--;
            intermediate_position--;

            break;
        case DIR_NORTH:
            current_position += width;
            intermediate_position += width;

            break;
        case DIR_SOUTH:
            current_position -= width;
            intermediate_position -= width;

            break;

        default:
            current_position++;
            intermediate_position++;
            break;
        }

        return true;
    }

    // check is there is a queen on the destination edge
    for (unsigned int i = 0; i < c->nums_queens; i++)
    {
        if (c->queens[(1 - c->id)][i] != move->queen_dst)
        {
            return 1;
        }
    }

    // check is there is a queen on the source edge
    for (unsigned int i = 0; i < c->nums_queens; i++)
    {
        if (c->queens[c->id][i] == move->queen_src)
        {
            return 0;
        }
    }

    return 1;
}

char const *get_player_name(void)
{
    return c->name;
}

void initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
    if (!c)
    {
        struct random_client *c = malloc(sizeof(c));

        c->name = "Random";
        c->id = player_id;
        c->graph = graph;
        c->nums_queens = num_queens;
        c->queens = queens;
    }
}

struct move_t play(struct move_t previous_move)
{
    return previous_move;
}

void finalize()
{
    destroy_graph(c->graph);
    free(c);
}