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



double minmax(node_t *node, unsigned int depth, bool maxiPlayer, board_t *board, unsigned int current_player){
    // printf("minmax depth = %u\n", depth);
    if (node->move.arrow_dst != -1) {
        // display_move(node->move);
        // printf(" by minmax\n");
        apply_move(board, &(node->move), current_player);
    }
    if (depth == 0 || isLeaf(node)){
        double value = power_heuristic(board, current_player);
        // board_print(board);
        // printf("heuristic is %lf for p%u\n", value.value, current_player);
        undo_move(board, &(node->move), current_player);
        return value;
    }   
    if(maxiPlayer){
    // board_print(board);
        double child_value = -INFINITY;
        for (size_t i = 0; i < node->child_count; i++)
        {
            double new_child_value = minmax(node->childs[i], depth - 1, false, board, 1 - current_player);
            if (new_child_value > child_value) child_value = new_child_value;
        }
        // board_print(board);
        // printf("depth %u, maxi = %d\n\t", depth, maxiPlayer);
        // display_node(node);

        if(node->move.arrow_dst != -1) undo_move(board, &(node->move), current_player);
        return child_value;
    }else{
        double child_value = INFINITY;
        for (size_t i = 0; i < node->child_count; i++)
        {
            double new_child_value = minmax(node->childs[i], depth - 1, true, board, 1 - current_player);
            if (new_child_value < child_value) child_value = new_child_value;
        }
        // board_print(board);
        // printf("depth %u, maxi = %d\n\t", depth, maxiPlayer);
        // display_node(node);

        if(node->move.arrow_dst != -1) undo_move(board, &(node->move), current_player);
        return child_value;
    }
}

struct move_t get_best_heuristic_move(board_t *board, unsigned int current_player){
    struct move_t best_move = {-1, -1, -1};
    double board_heuristic = -INFINITY;
    double best_move_heuristic = -INFINITY;

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
            queen_available_moves(c->board, &arrow_moves, queen_destination);

            //for every position that a moved queen can fire an arrow to
            for (unsigned int k = 0; k < arrow_moves.move_count; k++)
            {
                total_possible_state_count++;
                board_add_arrow(board, arrow_moves.indexes[k]);
                
                //get new heuristic
                node_t *game_tree = create_moves_tree(board, current_player);
                for (size_t i = 0; i < game_tree->child_count; i++)
                // {
                //     // display_node(game_tree->childs[i]);
                //     struct move_t previous = game_tree->childs[i]->move; 
                //     apply_move(c->board, &(game_tree->childs[i]->move),  c->id);
                //     // printf("creating sub tree\n");
                //     game_tree->childs[i] = create_moves_tree(c->board, 1 - c->id);
                //     game_tree->childs[i]->move = previous;     
                //     // printf("created sub tree\n");
                //     undo_move(c->board, &(game_tree->childs[i]->move),  c->id);
                //     game_tree->childs[i]->parent = game_tree;
                // }
                // printf("computing %u\n", total_possible_state_count);
                board_heuristic = minmax(game_tree, 1, true, board, 1 - current_player);
                destroy_tree(game_tree);

                //determines if the new one is better than the best 
                if (board_heuristic > best_move_heuristic || (board_heuristic == best_move_heuristic && rand()%3==0)){
                    // printf("Found better heuristic : from %lf to %lf\n", best_move_heuristic, board_heuristic);
                    //switch if necessary
                    best_move_heuristic = board_heuristic;
                    best_move.queen_src = queen_source;
                    best_move.queen_dst = queen_destination;
                    best_move.arrow_dst = arrow_moves.indexes[k];
                }

                //reset board by removing arrow
                board_remove_arrow(board, arrow_moves.indexes[k]);
            }

            //resets board by moving queen back to its old position
            queens_move(board->queens[current_player], board->board_width, queen_destination, queen_source);
        }
    }
    free(queen_moves.indexes);
    free(arrow_moves.indexes);

    // printf("Computed %u possibles states\n", total_possible_state_count);

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


    // for (size_t i = 0; i < game_tree->child_count; i++)
    // {
    //     // display_node(game_tree->childs[i]);
    //     struct move_t previous = game_tree->childs[i]->move; 
    //     apply_move(c->board, &(game_tree->childs[i]->move),  c->id);
    //     // printf("creating sub tree\n");
    //     game_tree->childs[i] = create_moves_tree(c->board, 1 - c->id);
    //     game_tree->childs[i]->move = previous;
    //     for (size_t j = 0; j < game_tree->childs[i]->child_count; j++)
    //     {
    //         struct move_t previous2 = game_tree->childs[i]->childs[j]->move; 
    //         apply_move(c->board, &(game_tree->childs[i]->childs[j]->move), 1-c->id);
    //         // printf("creating sub tree\n");
    //         game_tree->childs[i]->childs[j] = create_moves_tree(c->board, c->id);
    //         game_tree->childs[i]->childs[j]->move = previous2;
    //         undo_move(c->board, &(game_tree->childs[i]->childs[j]->move), 1 - c->id);
    //         game_tree->childs[i]->childs[j]->parent = game_tree->childs[i];
    //     }

    //     // printf("created sub tree\n");
    //     undo_move(c->board, &(game_tree->childs[i]->move),  c->id);
    //     game_tree->childs[i]->parent = game_tree;
    // }
    
    // printf("now minmaxing\n");
    
    struct move_t next_move = get_best_heuristic_move(c->board, c->id);
    //  minmax(game_tree, 3, true, c->board, 1 - c->id).index]->move;

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

