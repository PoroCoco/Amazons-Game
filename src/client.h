#ifndef _AMAZON_CLIENT_H_
#define _AMAZON_CLIENT_H_


#define NUM_PLAYERS 2

#include "board.h"
#include "queue.h"
/// \cond
#include <stdbool.h>
/// \endcond


typedef struct client{
    char *name;
    int id;
    board_t *board;
} client_t;

#endif // _AMAZON_CLIENT_H_
