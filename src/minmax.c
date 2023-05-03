#include <assert.h>



//node header







//header

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