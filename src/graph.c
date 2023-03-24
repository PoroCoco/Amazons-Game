#include "matrix.h"
#include "dir.h"


unsigned int get_neighbor(unsigned int m, unsigned int idx, enum dir_t d) 
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

struct graph_t *create_graph(unsigned int m, enum graph_type shape)
{
    struct graph_t *graph = malloc(1 * sizeof(struct graph_t));
    gsl_spmatrix_uint *mat = gsl_spmatrix_uint_alloc(m, m);
    
    switch (shape)
    {
    case SQUARE:
        graph->num_vertices=m*m;

        for (size_t vertex=0; vertex<graph->num_vertices; vertex++)
            for (enum dir_t dir=FIRST_DIR; dir<LAST_DIR; dir++)
                gsl_spmatrix_uint_set(mat, vertex, get_neighbor(m,vertex,dir), dir);
        
        graph->t = mat;
        break;

    default:
        break;
    }

    return graph;
}

void destroy_graph(struct graph_t *g)
{
    gsl_spmatrix_uint_free(g->t);
    free(g);
}

void print_graph(struct graph_t* g)
{
    gsl_spmatrix_uint_fprintf(stdout, g->t, "%u");
    for (size_t i = 0; i < g->t->size1; i++)
    {
        for (size_t j = 0; j < g->t->size2; j++)
            printf("%u ",gsl_spmatrix_uint_get(g->t, i, j));
        printf("\n");
    }
}