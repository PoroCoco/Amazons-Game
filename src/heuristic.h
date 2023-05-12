#ifndef _AMAZON_HEURISTIC_H_
#define _AMAZON_HEURISTIC_H_

#include "board.h"
#include "territories.h"
#include <math.h>



struct heuristic_data{
    double variance;
    double heuristic;
};

double power_heuristic(board_t *board, unsigned int player_id);
double power_heuristic_safe(board_t *board, unsigned int player_id);
double territory_heuristic_average(board_t *board, unsigned int player_id);
double improved_territory(board_t *board, unsigned int player_id);

struct heuristic_data automatic_territory(board_t *board, unsigned int player_id);
struct heuristic_data automatic_explosion(board_t *board, unsigned int player_id);



#endif // _AMAZON_HEURISTIC_H
