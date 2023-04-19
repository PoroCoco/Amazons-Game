#include "client_random.h"
#include "board.h"
#include "queens.h"
#include "move_logic.h"
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

struct move_t get_random_move()
{
    struct move_t next_move;
    queen_moves_t queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int)*c->board->board_cells*c->board->board_cells );
    
    next_move.queen_src = c->board->queens[c->id][rand() % (c->board->queens_count)];
    queen_available_moves(c->board, &queen_moves, next_move.queen_src);

    while (queen_moves.move_count == 0) {
        next_move.queen_src = c->board->queens[c->id][rand() % (c->board->queens_count)];
        queen_available_moves(c->board, &queen_moves, next_move.queen_src);
    }

    unsigned int new_dst = queen_moves.indexes[rand() % (queen_moves.move_count)];

    for (unsigned int i = 0; i < c->board->queens_count; i++)
    {
        if (c->board->queens[c->id][i] == new_dst || c->board->queens[1 - c->id][i] == new_dst )
        {
            i = 0;
            new_dst = queen_moves.indexes[rand() % (queen_moves.move_count)];
        }
    }
    next_move.queen_dst = new_dst;
    queens_move(c->board->queens[c->id], c->board->board_width, next_move.queen_src, next_move.queen_dst);
    queen_available_moves(c->board, &queen_moves, next_move.queen_dst);
    next_move.arrow_dst = queen_moves.indexes[rand() % (queen_moves.move_count)];
    queens_move(c->board->queens[c->id], c->board->board_width, next_move.queen_dst, next_move.queen_src);


    free(queen_moves.indexes);
    return next_move;
}

struct move_t play(struct move_t previous_move)
{
    // board_print(c->board);
    if (previous_move.arrow_dst != UINT_MAX && previous_move.queen_src != UINT_MAX && previous_move.queen_dst != UINT_MAX)
    {
        unsigned int index = 0;
        while (index < c->board->queens_count - 1 && c->board->queens[1 - c->id][index] != previous_move.queen_src)
            index++;

        c->board->queens[1 - c->id][index] = previous_move.queen_dst;
        board_add_arrow(c->board, previous_move.arrow_dst);
    }

    struct move_t next_move = get_random_move();

    // printf("my queen at %d move are : \n", c->board->queens[c->id][0]);

    while (!is_move_valid(c->board, &next_move, c->id))
    {
        // printf("my queens are : ");
        // for (size_t i = 0; i < c->board->queens_count; i++)
        // {
            // printf("%d ", c->board->queens[c->id][i]);
        // }
        // printf("\n");
        // printf("move invalid : %d, %d, %d\n", next_move.queen_src, next_move.queen_dst, next_move.arrow_dst);
  
        next_move = get_random_move();
    }

    // printf("src : %d\n", next_move.queen_src);
    // printf("dst : %d\n", next_move.queen_dst);
    // printf("arrow : %d\n", next_move.arrow_dst);

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
    printf("finalize for me client id %u, my ptr is %p\n", c->id, c);
    board_free(c->board);
    free(c);
}