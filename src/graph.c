#include "graph.h"
#include "dir.h"


unsigned int get_neighbor(int m, unsigned int idx, enum dir_t d) 
{
    if (is_dir_in_relation(d) == 0)
        return UINT_MAX;
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

struct graph_t make_square_graph(int m)
{
    gsl_spmatrix_uint *mat = gsl_spmatrix_uint_alloc(m, m);

    for (int i=0; i<m*m; i++)
        for (enum dir_t dir=FIRST_DIR; dir<LAST_DIR; dir++)
            gsl_spmatrix_uint_set(mat, i, get_neighbor(m,i,dir), dir);
}