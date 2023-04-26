#include "client_power_heuristic.h"
#include "board.h"
#include "queens.h"
#include "move_logic.h"
#include "heuristic.h"
#include "tree.h"

#include <math.h>
#include <assert.h>



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
        c->name = "MinMax";
        c->id = player_id;
        c->board = board_create(graph, queens, num_queens);
    }
}

node_t *create_moves_tree(board_t *board, unsigned int current_player){
    node_t *root = create_empty_node(NULL);
    queen_moves_t queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int)*c->board->board_cells*c->board->board_cells);
    assert(queen_moves.indexes);

    queen_moves_t arrow_moves;
    arrow_moves.indexes = malloc(sizeof(unsigned int)*c->board->board_cells*c->board->board_cells);
    assert(arrow_moves.indexes);


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
            queen_available_moves(c->board, &arrow_moves, queen_destination);

            //for every position that a moved queen can fire an arrow to
            for (unsigned int k = 0; k < arrow_moves.move_count; k++)
            {
                node_t *new_move_node = create_empty_node(root);
                struct move_t new_move = {  .arrow_dst = arrow_moves.indexes[k],
                                            .queen_src = queen_source,
                                            .queen_dst = queen_destination};
                new_move_node->move = new_move;
                add_child(root, new_move_node);
            }

            //resets board by moving queen back to its old position
            queens_move(board->queens[current_player], board->board_width, queen_destination, queen_source);
        }
    }

    free(queen_moves.indexes);
    free(arrow_moves.indexes);
    return root;
}


typedef struct searchInfo {
    double value;
    size_t index;
} searchInfo_t;

searchInfo_t minmax(node_t *node, unsigned int depth, board_t *board, unsigned int current_player){
    // printf("minmax depth = %u\n", depth);
    if (node->move.arrow_dst != -1) {
        // display_move(node->move);
        // printf(" by minmax\n");s
        apply_move(board, &(node->move), current_player);
    }
    if (depth == 0 || isLeaf(node)){
        searchInfo_t child_info = {.value = power_heuristic(board, current_player), .index = 0};
        undo_move(board, &(node->move), current_player);
        return child_info;
    } 
    // board_print(board);
        searchInfo_t child_info = {.value = INFINITY, .index = -1};
        for (size_t i = 0; i < node->child_count; i++)
        {
            searchInfo_t new_child_info = minmax(node->childs[i], depth - 1, board, 1 - current_player);
            if (new_child_info.value < child_info.value){
                child_info = new_child_info;
                child_info.index = i;
            } 
        }
        // board_print(board);
        // display_node(node);

        if(node->move.arrow_dst != -1) undo_move(board, &(node->move), current_player);
        return child_info;

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

    node_t *game_tree = create_moves_tree(c->board, c->id); 
    // printf("created main tree\n");

    for (size_t i = 0; i < game_tree->child_count; i++)
    {
        // display_node(game_tree->childs[i]);
        struct move_t previous = game_tree->childs[i]->move; 
        apply_move(c->board, &(game_tree->childs[i]->move),  c->id);
        // printf("creating sub tree\n");
        game_tree->childs[i] = create_moves_tree(c->board, 1 - c->id);
        game_tree->childs[i]->move = previous;
        // printf("created sub tree\n");
        undo_move(c->board, &(game_tree->childs[i]->move),  c->id);
        game_tree->childs[i]->parent = game_tree;
    }
    
    // printf("now minmaxing\n");
    
    struct move_t next_move = game_tree->childs[minmax(game_tree, 2, c->board, 1 - c->id).index]->move;
    destroy_tree(game_tree);

    display_move(next_move);
    printf(" computed by minmax client\n");


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

