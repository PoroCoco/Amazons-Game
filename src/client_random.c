#include "client_random.h"
#include "board.h"
#include "queens.h"
#include "territories.h"
#include "move_logic.h"
#include "heuristic.h"
#include <math.h>
#include <assert.h>
#include <pthread.h>

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
        c->name = "Client Random";
        c->id = player_id;
        c->board = board_create(graph, queens, num_queens);
    }
}

typedef struct {
    board_t *board;
    unsigned int current_player;
    unsigned int queen_id;
    struct move_t best_move;
    double best_move_heuristic;
} ThreadData;


void *threaded_queen_best_move(void *thread_args){
    ThreadData *data = (ThreadData *)thread_args;
    board_t *board = data->board;
    unsigned int current_player = data->current_player; 
    unsigned int queen_id = data->queen_id;

    double board_heuristic = -INFINITY;

    queen_moves_t queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells);
    assert(queen_moves.indexes);

    queen_moves_t arrow_moves;
    arrow_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells);
    assert(arrow_moves.indexes);

    unsigned int queen_source = board->queens[current_player][queen_id];
    queen_available_moves(board, &queen_moves, queen_source);

    //for every position that a queen can move to
    for (unsigned int j = 0; j < queen_moves.move_count; j++)
    {
        unsigned int queen_destination = queen_moves.indexes[j];
        queens_move(board->queens[current_player], board->board_width, queen_source, queen_destination);
        queen_available_moves(board, &arrow_moves, queen_destination);

        //for every position that a moved queen can fire an arrow to
        for (unsigned int k = 0; k < arrow_moves.move_count; k++)
        {
            board_add_arrow(board, arrow_moves.indexes[k]);
            
            //get new heuristic
                board_heuristic = territory_heuristic_average(board, current_player, get_territory_queen_move);
                // printf("Thread %u computed heuristic %lf\n", queen_id, board_heuristic);
            //determines if the new one is better than the best 
            if (board_heuristic > data->best_move_heuristic || (board_heuristic == data->best_move_heuristic && rand()%3 == 0)){
                // printf("Found better heuristic : from %lf to %lf\n",best_move_heuristic, board_heuristic);
                //switch if necessary
                data->best_move_heuristic = board_heuristic;
                data->best_move.queen_src = queen_source;
                data->best_move.queen_dst = queen_destination;
                data->best_move.arrow_dst = arrow_moves.indexes[k];
            }

            //reset board by removing arrow
            board_remove_arrow(board, arrow_moves.indexes[k]);
        }

        //resets board by moving queen back to its old position
        queens_move(board->queens[current_player], board->board_width, queen_destination, queen_source);
    }
    free(queen_moves.indexes);
    free(arrow_moves.indexes);

    // printf("i'm thread %u finishing\n", queen_id);
    pthread_exit(NULL);
}
struct move_t get_best_heuristic_move(board_t *board, unsigned int current_player){
    //1 thread for each queen
    int num_threads = board->queens_count;
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    
    struct move_t move_undefined = {-1, -1, -1};
    //inits and launch each thread
    for (int i = 0; i < num_threads; i++) {
        
        thread_data[i].board = board_copy(board);


        thread_data[i].best_move = move_undefined;
        thread_data[i].best_move_heuristic = -INFINITY;
        thread_data[i].current_player = current_player;
        thread_data[i].queen_id = i;


        int rc = pthread_create(&threads[i], NULL, threaded_queen_best_move, (void *)&thread_data[i]);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    //wait for all the threads to finish
    for (int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < num_threads; i++){
        board_free(thread_data[i].board);
    }


    //get the best move of among the queens
    struct move_t best_move = thread_data[0].best_move;
    double best_move_heuristic = thread_data[0].best_move_heuristic;
    for (int i = 1; i < num_threads; i++){
        if (thread_data[i].best_move_heuristic > best_move_heuristic){
            best_move = thread_data[i].best_move;
            best_move_heuristic = thread_data[i].best_move_heuristic;
        }
    }

    return best_move;
}

struct move_t play(struct move_t previous_move)
{
    if (previous_move.arrow_dst != UINT_MAX && previous_move.queen_src != UINT_MAX && previous_move.queen_dst != UINT_MAX)
    {
        unsigned int index = 0;
        while (index < c->board->queens_count - 1 && c->board->queens[1 - c->id][index] != previous_move.queen_src)
            index++;

        c->board->queens[1 - c->id][index] = previous_move.queen_dst;
        board_add_arrow(c->board, previous_move.arrow_dst);
    }

    struct move_t next_move = get_best_heuristic_move(c->board, c->id);

    unsigned int index = 0;
    while (index < c->board->queens_count - 1 && c->board->queens[c->id][index] != next_move.queen_src)
        index++;

    c->board->queens[c->id][index] = next_move.queen_dst;
    board_add_arrow(c->board, next_move.arrow_dst);
    // board_print(c->board);
    return next_move;
}

void finalize(void)
{
    // printf("finalize for me client id %u, my ptr is %p\n", c->id, c);
    board_free(c->board);
    free(c);
}