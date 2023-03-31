#include "client_random.h"

struct random_client *c = NULL;

enum dir_t get_move_direction(size_t origin, size_t destination, size_t width)
{
    if (origin == destination || !origin || !destination || !width)
        return -1;

    if (abs(origin - destination) < width)
    {

        return (int)origin - (int)destination > 0 ? DIR_EAST : DIR_WEST;
    }
    else
    {
        if ((destination % width) == (origin % width))
        {
            return (int)origin - (int)destination > 0 ? DIR_NORTH : DIR_SOUTH;
        }

        return -1;
    }
}

int is_move_valid(struct random_client *c, struct move_t *move)
{

    int direction = get_move_direction(move->queen_src, move->queen_dst, board_width(c->graph));

    // check if destination edge exist
    for (unsigned int i = move->queen_src; i < move->queen_dst;)
    {
        
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

struct move_t play(struct move_t previous_move);

void finalize()
{
    free(c->graph);
    free(c);
}