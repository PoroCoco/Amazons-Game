#include "graph_ext.h"
#include "dir.h"
#include "test.h"

/*
enum dir_t {
  NO_DIR=0,
  DIR_NORTH=1, DIR_NE=2,
  DIR_EAST=3,  DIR_SE=4,
  DIR_SOUTH=5, DIR_SW=6,
  DIR_WEST=7,  DIR_NW=8,
  FIRST_DIR=DIR_NORTH, LAST_DIR=DIR_NW,
  NUM_DIRS=8,
  DIR_ERROR
};
*/

int test_create_and_free_square_graph(void) {
    struct graph_t* graph = create_graph(2, SQUARE);

    enum dir_t expected_dir[4][4] = {{NO_DIR, DIR_EAST, DIR_SOUTH, NO_DIR}, 
                                    {DIR_WEST, NO_DIR, NO_DIR, DIR_SOUTH},
                                    {DIR_NORTH, NO_DIR, NO_DIR, DIR_EAST}, 
                                    {NO_DIR, DIR_NORTH, DIR_WEST, NO_DIR}};
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            assert(expected_dir[i][j] == gsl_spmatrix_uint_get(graph->t,i,j));    
    //print_graph(graph);
    destroy_graph(graph);
    return 0;

}
