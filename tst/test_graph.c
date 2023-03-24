#include "matrix.h"
#include "test.h"


int test_square_graph(void) {
    struct graph_t* graph = create_graph(4,SQUARE);
    print_graph(graph);
    destroy_graph(graph);
    return 1;

}
