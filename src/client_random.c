#include "client_random.h"
#include "board.h"
#include "queens.h"
#include <math.h>
#include <assert.h>

struct random_client *c = NULL;


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
    board_print(c->board);
    return previous_move;
}

void finalize(void)
{
    printf("finalize for me client id %u, my ptr is %p\n", c->id, c);
    board_free(c->board);
    free(c);
}