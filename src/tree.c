#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct node *node_create(int win, int game,
                         struct board *current_board,
                         struct move_t *previous_move)
{
    struct node *node = malloc(sizeof(struct node));
    node->nb_wins = win;
    node->nb_game = game;

    node->current_board = current_board;
    node->move = previous_move;

    node->parent = NULL;
    node->childs = NULL;
    node->childs_count = 0;
    return node;
}

// Optimisation possible en changeant la méthode d'allocation mémoire
int node_add(struct node *parent, struct node *node)
{
    assert(parent != NULL);
    assert(node != NULL);

    parent->childs_count++;
    parent->childs = realloc(parent->childs, sizeof(parent) * parent->childs_count);
    parent->childs[(parent->childs_count) - 1] = node;

    node->parent = parent;
    return 1;
}

void node_free(struct node *node) {
    for (unsigned int i = 0; i < node->childs_count; i++) {
        if (node->childs[i]->childs_count != 0) {
            for (unsigned int j = 0; j < node->childs[i]->childs_count; j++) {
                node_free(node->childs[i]->childs[j]);
            }
        }
        free(node->childs[(node->childs_count) - 1]);
    }
    free(node->childs);
    free(node);
}
