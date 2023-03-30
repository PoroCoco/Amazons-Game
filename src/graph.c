#include "graph_ext.h"
#include "dir.h"
#include "assert.h"
#include "string.h"


size_t get_neighbor(unsigned int m, unsigned int idx, enum dir_t d) 
{
    if ((idx < m) && ((d == DIR_NE) || (d == DIR_NORTH) || (d == DIR_NW)))
        return UINT_MAX;
    if ((idx % m == 0) && ((d == DIR_NW) || (d == DIR_WEST) || (d == DIR_SW)))
        return UINT_MAX;
    if ((idx % m == m -1) && ((d == DIR_NE) || (d == DIR_EAST) || (d == DIR_SE)))
        return UINT_MAX;
    if ((idx >= m*(m-1)) && ((d == DIR_SW) || (d == DIR_SOUTH) || (d == DIR_SE)))
        return UINT_MAX;
    else 
    {
        switch (d)
        {
            case DIR_NW:
                return (idx - m - 1);
            case DIR_NORTH:
                return (idx - m);
            case DIR_NE:
                return (idx - m + 1);
            case DIR_WEST:
                return (idx - 1);
            case DIR_EAST:
                return (idx + 1);
            case DIR_SW:
                return (idx + m - 1);
            case DIR_SOUTH:
                return (idx + m);
            case DIR_SE:
                return (idx + m + 1);
            default:
                return UINT_MAX;
        }
    }
}

// return the direction of v1 --> v2
enum dir_t get_dir(unsigned int m, unsigned int v1, unsigned int v2)
{

    if (v2 == v1 - m - 1)
        return DIR_NW;
    if (v2 == v1 - m)
        return DIR_NORTH;
    if (v2 == v1 - m + 1)
        return DIR_NE;
    if (v2 == v1 - 1)
        return DIR_WEST;
    if (v2 == v1 + 1)
        return DIR_EAST;
    if (v2 == v1 + m - 1)
        return DIR_SW;
    if (v2 == v1 + m)
        return DIR_SOUTH;
    if (v2 == v1 + m + 1)
        return DIR_SE;
    else
        return NO_DIR;

}

struct graph_t *create_graph(unsigned int m, enum graph_type shape)
{
    struct graph_t *graph = malloc(sizeof(struct graph_t));
    gsl_spmatrix_uint *mat = gsl_spmatrix_uint_alloc(m*m, m*m);
    
    switch (shape)
    {
    case SQUARE:
        graph->num_vertices=m*m;

        for (size_t vertex=0; vertex<graph->num_vertices; vertex++){
            //iteration on all cardinal directions
            for (enum dir_t dir = FIRST_DIR; dir<LAST_DIR; dir += 2){
                size_t neighbor = get_neighbor(m, vertex, dir);
                if (neighbor == UINT_MAX) continue;
                gsl_spmatrix_uint_set(mat, vertex, neighbor, dir);
            } 
        }
        break;

    default:
        break;
    }

    //converting the matrix to the CSR format
    gsl_spmatrix_uint *csr = gsl_spmatrix_uint_compress(mat, GSL_SPMATRIX_CSR);
    assert(strcmp(gsl_spmatrix_uint_type(csr), "CSR") == 0);
    gsl_spmatrix_uint_free(mat);

    graph->t = csr;

    return graph;
}

void destroy_graph(struct graph_t *g)
{
    gsl_spmatrix_uint_free(g->t);
    free(g);
}

void print_graph(const struct graph_t* g)
{
    for (size_t i = 0; i < g->t->size2; i++)
    {
        for (size_t j = 0; j < g->t->size1; j++)
            printf("%u ",gsl_spmatrix_uint_get(g->t, i, j));
        printf("\n");
    }
}


void add_edge(struct graph_t *g, size_t v1, size_t v2, unsigned int value){
    if (gsl_spmatrix_uint_set(g->t, v1, v2, value) != 0){
        fprintf(stderr, "Failed to add an edge with value %u from vertex %lu to %lu\n", value, v1, v2);
        exit(EXIT_FAILURE);
    }
}

enum graph_type convert_char_to_shape(char shape){
    return 0;
}

struct graph_t* graph_copy(const struct graph_t* g){
    return NULL;
}
