#include <assert.h>
#include "alphabeta.h"
#include "heuristic.h"
#include "queens.h"
#include "territories.h"



typedef struct node{
    struct node **childs;
    struct node *parent;
    size_t child_count;
    size_t allocated;
    struct move_t move;
} node_t;

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
            board->queen_occupy[queen_source] = false;
            board->queen_occupy[queen_destination] = true;
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
            board->queen_occupy[queen_source] = true;
            board->queen_occupy[queen_destination] = false;
        }
    }

    free(queen_moves.indexes);
    free(arrow_moves.indexes);
    return root;
}

double alphabeta(double alpha, double beta, board_t *board, unsigned int current_player, bool maxiPlayer, unsigned int original_player, unsigned int depth){
    if (depth == 0){
        return territory_heuristic_average(board, current_player);
    }
    queen_moves_t queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells);
    assert(queen_moves.indexes);

    queen_moves_t arrow_moves;
    arrow_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells);
    assert(arrow_moves.indexes);


    double value = INFINITY;
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
            board->queen_occupy[queen_source] = false;
            board->queen_occupy[queen_destination] = true;
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
                    double new_value = alphabeta(alpha, beta, board, 1 - current_player, !maxiPlayer, original_player, depth - 1);
                    if (new_value < value) value = new_value;
                    if (alpha >= value){
                        board_remove_arrow(board, new_move.arrow_dst);
                        break;
                    }
                    beta = beta > value ? value : beta;
                }else{
                    value = -INFINITY;
                    double new_value = alphabeta(alpha, beta, board, 1 - current_player, !maxiPlayer, original_player, depth - 1);
                    if (new_value > value) value = new_value;
                    if (value >= beta){
                        board_remove_arrow(board, new_move.arrow_dst);
                        break;
                    }
                    alpha = alpha > value ? alpha : value;
                }
                board_remove_arrow(board, new_move.arrow_dst);
            }

            //resets board by moving queen back to its old position
            queens_move(board->queens[current_player], board->board_width, queen_destination, queen_source);
            board->queen_occupy[queen_source] = true;
            board->queen_occupy[queen_destination] = false;
        }
    }

    //node is a leaf
    free(arrow_moves.indexes);
    free(queen_moves.indexes);
    double node_value = territory_heuristic_average(board, current_player);


    if (!maxiPlayer){
        if (node_value < value) value = node_value;
    }else{
        if (node_value > value) value = node_value;
    }
    return value;
}

struct move_t get_move_alphabeta(board_t *board, unsigned int current_player){
    double board_heuristic = -INFINITY;
    double best_move_heuristic = -INFINITY;
    struct move_t best_move = {-1, -1, -1};

    node_t * game_tree = create_moves_tree(board, current_player, 2, NULL);
    for (size_t i = 0; i < game_tree->child_count; i++)
    {
        apply_move(board, &(game_tree->childs[i]->move), current_player);
        board_heuristic = alphabeta(-INFINITY, INFINITY, board, current_player, true, current_player, 2);
        undo_move(board, &(game_tree->childs[i]->move), current_player);

        //determines if the new one is better than the best 
        if (board_heuristic > best_move_heuristic || (board_heuristic == best_move_heuristic && rand()%3==0)){
            //switch if necessary
            best_move_heuristic = board_heuristic;
            best_move = game_tree->childs[i]->move;
        }
    }
    destroy_tree(game_tree);
    return best_move;
}