#include "client_random.h"
#include <math.h>

struct random_client *c = NULL;

enum dir_t get_direction(size_t origin, size_t destination) {
    
}

int is_move_valid(struct random_client *c, struct move_t *move)
{
    // check if destination edge exist
    for (unsigned int i = move->queen_src; i < move->queen_dst; i++)
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

unsigned int board_width(struct graph_t *graph){
    return (unsigned int) sqrt(graph->t->size1);
}

void initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
    if (c == NULL)
    {
        c = malloc(sizeof(struct random_client));
        c->name = "Random";
        c->id = player_id;
        c->graph = graph;
        c->nums_queens = num_queens;
        c->queens = queens;
    }
}

struct move_t play(struct move_t previous_move){
    return previous_move;
}

void finalize()
{
    printf("finalize for me client id %u, my ptr is %p\n", c->id, c);
    // destroy_graph(c->graph);
    // free(c);
}