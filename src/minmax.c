#include <assert.h>
#include "minmax.h"
#include "heuristic.h"
#include "queens.h"
#include "territories.h"

bool isLeaf(node_t *node){
    return node->child_count == 0;
}


node_t *create_empty_node(node_t *parent){
    node_t *empty_node = malloc(sizeof(*empty_node));
    assert(empty_node);

    empty_node->parent = parent;
    struct move_t move = {-1, -1, -1}; 
    empty_node->move = move;
    empty_node->child_count = 0;
    empty_node->allocated = 128;
    empty_node->childs = malloc(sizeof(*empty_node->childs) * empty_node->allocated);
    assert(empty_node->childs);


    return empty_node;
}

void add_child(node_t *node, node_t *child){
    if(child == NULL || node == child) return;
    if (node->child_count == node->allocated){
        node->allocated *= 2;
        node->childs = realloc(node->childs, sizeof(*node->childs) * node->allocated);
        assert(node->childs);
    }
    child->parent = node;
    node->childs[node->child_count] = child;

    node->child_count++;
}

void destroy_tree(node_t *root){
    assert(root);
    for (size_t i = 0; i < root->child_count; i++)
    {
        destroy_tree(root->childs[i]);
    }

    free(root->childs);
    free(root);
}

void _display_move(struct move_t m){
    printf("{arrow = %u, queen_src = %u, queen_dst = %u}", m.arrow_dst, m.queen_src, m.queen_dst);
}
void display_node(node_t *node){
    printf("node : child_count = %zu, allocated_child = %zu, move =", node->child_count, node->allocated);
    _display_move(node->move);
    printf("\n");
}


node_t *create_moves_tree(board_t *board, unsigned int current_player, unsigned int depth, node_t *root){
    if (depth == 0){
        if (root != NULL) destroy_tree(root);
        return NULL;
    }
    if (root == NULL){
        root = create_empty_node(NULL);
    }
    queen_moves_t queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells);
    assert(queen_moves.indexes);

    queen_moves_t arrow_moves;
    arrow_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells);
    assert(arrow_moves.indexes);


    //for every queen of the player
    for (unsigned int i = 0; i < board->queens_count; i++)
    {
        unsigned int queen_source = board->queens[current_player][i];
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
                node_t *new_move_node = create_empty_node(root);
                struct move_t new_move = {  .arrow_dst = arrow_moves.indexes[k],
                                            .queen_src = queen_source,
                                            .queen_dst = queen_destination};
                new_move_node->move = new_move;
                board_add_arrow(board, new_move.arrow_dst);
                add_child(root, create_moves_tree(board, 1 - current_player, depth - 1, new_move_node));
                board_remove_arrow(board, new_move.arrow_dst);
            }

            //resets board by moving queen back to its old position
            queens_move(board->queens[current_player], board->board_width, queen_destination, queen_source);
        }
    }

    free(queen_moves.indexes);
    free(arrow_moves.indexes);
    return root;
}

double minmax(node_t *node, unsigned int depth, bool maxiPlayer, board_t *board, unsigned int current_player, unsigned int original_player){
    apply_move(board, &(node->move), current_player);
    if (depth == 0 || isLeaf(node)){
        double value = power_heuristic(board, original_player);
        // board_print(board);
        // printf("heuristic is %lf for p%u\n", value, current_player);
        undo_move(board, &(node->move), current_player);
        return value;
    }   
    if(maxiPlayer){
    // board_print(board);
        double child_value = -INFINITY;
        for (size_t i = 0; i < node->child_count; i++)
        {
            double new_child_value = minmax(node->childs[i], depth - 1, false, board, 1 - current_player, original_player);
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
            double new_child_value = minmax(node->childs[i], depth - 1, true, board, 1 - current_player, original_player);
            if (new_child_value < child_value) child_value = new_child_value;
        }
        // board_print(board);
        // printf("depth %u, maxi = %d\n\t", depth, maxiPlayer);
        // display_node(node);

        if(node->move.arrow_dst != -1) undo_move(board, &(node->move), current_player);
        return child_value;
    }
}

struct move_t get_move_minmax(node_t *game_tree, board_t *board, unsigned int current_player){
    double board_heuristic = -INFINITY;
    double best_move_heuristic = -INFINITY;
    struct move_t best_move = {-1, -1, -1};

    for (size_t i = 0; i < game_tree->child_count; i++)
    {
        board_heuristic = minmax(game_tree->childs[i], 10, true, board, current_player, current_player);

        //determines if the new one is better than the best 
        if (board_heuristic > best_move_heuristic || (board_heuristic == best_move_heuristic && rand()%3==0)){
            // printf("Found better heuristic : from %lf to %lf\n",best_move_heuristic, board_heuristic);
            //switch if necessary
            best_move_heuristic = board_heuristic;
            best_move = game_tree->childs[i]->move;
        }
    }
    return best_move;
}

double alphabeta(double alpha, double beta, board_t *board, unsigned int current_player, bool maxiPlayer, unsigned int original_player){

    queen_moves_t queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells);
    assert(queen_moves.indexes);

    queen_moves_t arrow_moves;
    arrow_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells);
    assert(arrow_moves.indexes);


    double value;
    //for every queen of the player
    for (unsigned int i = 0; i < board->queens_count; i++)
    {
        unsigned int queen_source = board->queens[current_player][i];
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
                struct move_t new_move = {  .arrow_dst = arrow_moves.indexes[k],
                                            .queen_src = queen_source,
                                            .queen_dst = queen_destination};
                board_add_arrow(board, new_move.arrow_dst);

                if (!maxiPlayer){
                    value = INFINITY;
                    double new_value = alphabeta(alpha, beta, board, 1 - current_player, !maxiPlayer, original_player);
                    if (new_value < value) value = new_value;
                    if (alpha >= value){
                        board_remove_arrow(board, new_move.arrow_dst);
                        queens_move(board->queens[current_player], board->board_width, queen_destination, queen_source);
                        free(queen_moves.indexes);
                        free(arrow_moves.indexes);
                        return value;
                    }
                    beta = beta > value ? value : beta;
                }else{
                    double new_value = alphabeta(alpha, beta, board, 1 - current_player, !maxiPlayer, original_player);
                    if (new_value > value) value = new_value;
                    if (value >= beta){
                        board_remove_arrow(board, new_move.arrow_dst);
                        queens_move(board->queens[current_player], board->board_width, queen_destination, queen_source);
                        free(queen_moves.indexes);
                        free(arrow_moves.indexes);
                        return value;
                    }
                    alpha = alpha > value ? alpha : value;
                }
                board_remove_arrow(board, new_move.arrow_dst);
            }

            //resets board by moving queen back to its old position
            queens_move(board->queens[current_player], board->board_width, queen_destination, queen_source);
        }
    }

    //node is a leaf
    free(arrow_moves.indexes);
    free(queen_moves.indexes);
    double node_value = power_heuristic(board, current_player);
    return node_value;
}

struct move_t get_move_alphabeta(board_t *board, unsigned int current_player){
    double board_heuristic = -INFINITY;
    double best_move_heuristic = -INFINITY;
    struct move_t best_move = {-1, -1, -1};

    node_t * game_tree = create_moves_tree(board, current_player, 2, NULL);
    for (size_t i = 0; i < game_tree->child_count; i++)
    {
        // apply_move(board, &(game_tree->childs[i]->move), current_player);
        board_heuristic = alphabeta(-INFINITY, INFINITY, board, current_player, true, current_player);

        //determines if the new one is better than the best 
        if (board_heuristic > best_move_heuristic || (board_heuristic == best_move_heuristic && rand()%3==0)){
            // printf("Found better heuristic : from %lf to %lf\n",best_move_heuristic, board_heuristic);
            //switch if necessary
            best_move_heuristic = board_heuristic;
            best_move = game_tree->childs[i]->move;
        }
    }
    destroy_tree(game_tree);
    return best_move;
}