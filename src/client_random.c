#include "client.h"
#include "queens.h"
#include "territories.h"
#include "heuristic.h"
/// \cond
#include <assert.h>
/// \endcond

struct client *c = NULL;


char const *get_player_name(void)
{
    return c->name;
}

void initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
    if (c == NULL)
    {
        c = malloc(sizeof(struct client));
        c->name = "Client Random";
        c->id = player_id;
        c->board = board_create(graph, queens, num_queens);
    }
}

struct move_t get_best_heuristic_move(board_t *board, unsigned int current_player){
    struct move_t best_move = {-1, -1, -1};
    struct heuristic_data best_move_heuristic = {
        .variance = -INFINITY,
        .heuristic = -INFINITY
    };

    struct heuristic_data board_heuristic = {
        .variance = -INFINITY,
        .heuristic = -INFINITY
    };
    queen_moves_t queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int)*c->board->board_cells*c->board->board_cells);
    assert(queen_moves.indexes);

    queen_moves_t arrow_moves;
    arrow_moves.indexes = malloc(sizeof(unsigned int)*c->board->board_cells*c->board->board_cells);
    assert(arrow_moves.indexes);

    unsigned int total_possible_state_count = 0;
    //for every queen of the player
    for (unsigned int i = 0; i < board->queens_count; i++)
    {
        unsigned int queen_source = board->queens[current_player][i];
        queen_available_moves(c->board, &queen_moves, queen_source);

        //for every position that a queen can move to
        for (unsigned int j = 0; j < queen_moves.move_count; j++)
        {
            unsigned int queen_destination = queen_moves.indexes[j];
            queens_move(board->queens[current_player], board->board_width, queen_source, queen_destination);
            board->queen_occupy[queen_source] = false;
            board->queen_occupy[queen_destination] = true;

            queen_available_moves(c->board, &arrow_moves, queen_destination);

            //for every position that a moved queen can fire an arrow to
            for (unsigned int k = 0; k < arrow_moves.move_count; k++)
            {
                total_possible_state_count++;
                board_add_arrow(board, arrow_moves.indexes[k]);
                
                //get new heuristic
                if(board->arrows_count > 1 && board->board_width < 11){
                    board_heuristic = automatic_territory(board, current_player);  
                }
                else
                    board_heuristic = automatic_explosion(board, current_player);
                
                //determines if the new one is better than the best 
                if (board_heuristic.heuristic > best_move_heuristic.heuristic){
<<<<<<< HEAD
=======
                    //printf("Found better heuristic : from %lf to %lf\n",best_move_heuristic.heuristic, board_heuristic.heuristic);
>>>>>>> 081f09d57c94de7b71ecf9ef2b9e1139a07eab78
                    //switch if necessary
                    best_move_heuristic.heuristic = board_heuristic.heuristic;
                    best_move_heuristic.variance = board_heuristic.variance;
                    best_move.queen_src = queen_source;
                    best_move.queen_dst = queen_destination;
                    best_move.arrow_dst = arrow_moves.indexes[k];
                }
<<<<<<< HEAD
=======
                else if((board_heuristic.heuristic == best_move_heuristic.heuristic)){

                    if(board_heuristic.variance < best_move_heuristic.variance){

                    //printf("Found better heuristic : from %f to %f\n",best_move_heuristic.variance, board_heuristic.variance);
                        best_move_heuristic.variance = board_heuristic.variance;
                        best_move.queen_src = queen_source;
                        best_move.queen_dst = queen_destination;
                        best_move.arrow_dst = arrow_moves.indexes[k];
                    }
                }
>>>>>>> 081f09d57c94de7b71ecf9ef2b9e1139a07eab78
                //reset board by removing arrow
                board_remove_arrow(board, arrow_moves.indexes[k]);
            }

            //resets board by moving queen back to its old position
            queens_move(board->queens[current_player], board->board_width, queen_destination, queen_source);
            board->queen_occupy[queen_destination] = false;
            board->queen_occupy[queen_source] = true;
        }
    }
    free(queen_moves.indexes);
    free(arrow_moves.indexes);


    return best_move;
}

struct move_t play(struct move_t previous_move)
{
    if (previous_move.arrow_dst != UINT_MAX && previous_move.queen_src != UINT_MAX && previous_move.queen_dst != UINT_MAX)
    {
        apply_move(c->board, &previous_move, 1 - c->id);
    }

    struct move_t next_move = get_best_heuristic_move(c->board, c->id);

    apply_move(c->board, &next_move, c->id);

    // board_print(c->board);
    return next_move;
}

void finalize(void)
{
    board_free(c->board);
    free(c);
}