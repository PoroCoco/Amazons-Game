#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "montecarlo.h"

struct mcts_client *c = NULL;

bool is_moves_equals(struct move_t* move1, struct move_t* move2)
{
    return ((move1->arrow_dst == move2->arrow_dst) && (move1->queen_dst == move2->queen_dst) && (move1->queen_src == move2->queen_src));
}

struct node* childs_has_move(struct node* current, struct move_t* move)
{
    for (size_t child_index = 0; child_index < current->childs_count; child_index++)
        if (is_moves_equals(current->childs[child_index]->move, move))
            return current->childs[child_index];
    return NULL;
}

char const *get_player_name(void)
{
    return c->name;
}

void initialize(unsigned int player_id, struct graph_t *graph,
                unsigned int num_queens, unsigned int *queens[NUM_PLAYERS])
{
    if (c == NULL)
    {
        c = malloc(sizeof(struct mcts_client));
        c->name = "Mcts_qui_bat_tout";
        c->id = player_id;
        c->board = board_create(graph, queens, num_queens);
    }
}

struct move_t* get_random_move(board_t* board, unsigned int id)
{
    struct move_t* next_move = malloc(1*sizeof(struct move_t));
    queen_moves_t queen_moves;
    queen_moves.indexes = malloc(sizeof(unsigned int)*board->board_cells*board->board_cells );
    
    next_move->queen_src = board->queens[id][rand() % (board->queens_count)];
    queen_available_moves(board, &queen_moves, next_move->queen_src);

    while (queen_moves.move_count == 0) {
        next_move->queen_src = board->queens[id][rand() % (board->queens_count)];
        queen_available_moves(board, &queen_moves, next_move->queen_src);
    }

    unsigned int new_dst = queen_moves.indexes[rand() % (queen_moves.move_count)];

    for (unsigned int i = 0; i < board->queens_count; i++)
    {
        if (board->queens[id][i] == new_dst || board->queens[1 - id][i] == new_dst )
        {
            i = 0;
            new_dst = queen_moves.indexes[rand() % (queen_moves.move_count)];
        }
    }
    next_move->queen_dst = new_dst;
    queens_move(board->queens[id], board->board_width, next_move->queen_src, next_move->queen_dst);
    queen_available_moves(board, &queen_moves, next_move->queen_dst);
    next_move->arrow_dst = queen_moves.indexes[rand() % (queen_moves.move_count)];
    queens_move(board->queens[id], board->board_width, next_move->queen_dst, next_move->queen_src);


    free(queen_moves.indexes);
    return next_move;
}


struct node* selection(struct node* root) 
{
    struct node* cur_node = root;

    if (cur_node->childs_count > 0)
    {
        struct node* max_node = cur_node->childs[0]; 
        double ucb = cur_node->childs[0]->nb_wins/(cur_node->childs[0]->nb_game+1) + sqrt(2)*sqrt(log(cur_node->nb_game+1))/(cur_node->childs[0]->nb_game+1);
        for (unsigned int childs_index = 1; childs_index < cur_node->childs_count; childs_index++)
        {
            double new_ucb = cur_node->childs[childs_index]->nb_wins/(cur_node->childs[childs_index]->nb_game+1) + sqrt(2)*sqrt(log(cur_node->nb_game))/(cur_node->childs[childs_index]->nb_game+1);
            if(ucb < new_ucb)
            {
                
                ucb = new_ucb;
                max_node = cur_node->childs[childs_index];
            }
            //printf("%lf\n",ucb);
        }
        apply_move(root->board, max_node->move, max_node->player);        
        cur_node = selection(max_node);

    }
    //printf("%lf\n",cur_node->nb_wins/cur_node->nb_game);
    return cur_node;
    
}

void expansion(struct node* parent,unsigned node_nb)
{
    if (!is_game_over_for_player(parent->board,1-parent->player))
    {
        struct node* new_node;
        
        for (size_t i=0; i<node_nb; i++)
        {
            struct move_t* move = get_random_move(parent->board,1-parent->player);
            while (childs_has_move(parent,move) != NULL)
                move = get_random_move(parent->board,1-parent->player);

            new_node = node_create(0,1,parent->board,move);
            node_add(parent,new_node);
        }
    }
}


bool simulation(struct node* current, unsigned int player_to_check_win)
{    
    board_t *copy = board_copy(current->board);
    while (!is_game_over_for_player(copy, 1-current->player))
    {
        struct move_t* next_move = get_random_move(copy, 1-current->player);
        apply_move(copy, next_move, 1-current->player);
        free(next_move);
    }
    if (is_game_over_for_player(copy, player_to_check_win)){
        board_free(copy);
        return false;
    }
    board_free(copy);
    return true;
}

void back_propagation(struct node* current ,bool has_win) {
    struct node* cursor = current;
    while (cursor->parent != NULL) {
        cursor->nb_game += 1;
        if (has_win)
            cursor->nb_wins +=1;
        cursor = cursor->parent;
    }
}
/*
void print_node(struct node* cur_node) {
    printf("%u/%u\n",cur_node->nb_wins,cur_node->nb_game);
    if (cur_node->childs_count > 0) {

    }
}
*/


struct move_t MCTS(struct board* current, unsigned int nb_expension) 
{
    
    struct node* root = node_create(0,1,current,NULL);
    root->player = 1-c->id;

    for (size_t exp = 0; exp<nb_expension; exp++) {
            struct node* selected_node = selection(root);
            expansion(selected_node,2);
            for (size_t nb_sim=0; nb_sim<10; nb_sim++) {
                struct node* rand_child_node = selected_node->childs[rand() % selected_node->childs_count];
                apply_move(current,rand_child_node->move,rand_child_node->player);
                back_propagation(selected_node, simulation(rand_child_node,c->id));
                undo_move(current, rand_child_node->move, rand_child_node->player);
            }
            while(selected_node->parent != NULL){
                undo_move(current, selected_node->move, selected_node->player);
                selected_node = selected_node->parent;
            }
    }


    double max_ratio = 0;
    unsigned int i_max = 0;
    for(unsigned int i = 0; i < root->childs_count; i++) {
        double ratio = root->childs[i]->nb_wins/(root->childs[i]->nb_game+1);
        if( ratio > max_ratio) {
            max_ratio = ratio;
            i_max = i;
        }
    }
    struct move_t r = {.arrow_dst = root->childs[i_max]->move->arrow_dst, .queen_dst = root->childs[i_max]->move->queen_dst, .queen_src = root->childs[i_max]->move->queen_src};
    return r;

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

    // printf("my queen at %d move are : \n", c->board->queens[c->id][0]);


    // printf("src : %d\n", next_move.queen_src);
    // printf("dst : %d\n", next_move.queen_dst);
    // printf("arrow : %d\n", next_move.arrow_dst);

    struct move_t next_move = MCTS(c->board,100);

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