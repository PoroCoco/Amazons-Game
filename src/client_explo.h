#ifndef _AMAZON_client_explo_H_
#define _AMAZON_client_explo_H_


#define NUM_PLAYERS 2

#include "board.h"

/// \cond
#include <stdbool.h>
/// \endcond


typedef struct client_explo{
    char *name;
    int id;
    board_t *board;
} client_explo_t;


client_explo_t *initialize_explo(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS]);
char const *get_player_name_explo(struct client_explo *c);
struct move_t play_explo(struct client_explo *c, struct move_t previous_move);
void finalize_explo(struct client_explo *c);
#endif // _AMAZON_client_explo_H_
