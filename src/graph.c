#include <assert.h>
#include <math.h>

#include "graph_ext.h"
#include "dir.h"
#include "string.h"

size_t get_neighbor(unsigned int m, unsigned int idx, enum dir_t d)
{
    if ((idx < m) && ((d == DIR_NE) || (d == DIR_NORTH) || (d == DIR_NW)))
        return UINT_MAX;
    if ((idx % m == 0) && ((d == DIR_NW) || (d == DIR_WEST) || (d == DIR_SW)))
        return UINT_MAX;
    if ((idx % m == m - 1) && ((d == DIR_NE) || (d == DIR_EAST) || (d == DIR_SE)))
        return UINT_MAX;
    if ((idx >= m * (m - 1)) && ((d == DIR_SW) || (d == DIR_SOUTH) || (d == DIR_SE)))
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

unsigned int get_width(const struct graph_t *g)
{
    double floating_width = sqrt(g->t->size1);
    assert((floating_width - floor(floating_width) == 0));
    return (unsigned int)floating_width;
}


int is_vertex_in_shape(unsigned int width, enum graph_type shape, unsigned int vertex) {
    int i = vertex / width;
    int j = vertex % width;

    switch (shape)
    {
        case DONUT:
            int aThird = width / 3;
            if ((aThird <= j) && (j < 2 * aThird))
                if ((aThird <= i) && (i < 2 * aThird))
                    return 0;
            break;
        case CLOVER:
            int aFifth = width / 5;
            if (((aFifth <= j) && (j < 2 * aFifth)) && ( ((1*aFifth <= i) && (i < 2*aFifth)) || ((3*aFifth <= i) && (i < 4*aFifth))))
                return 0;
            if (((3*aFifth <= j) && (j < 4 * aFifth)) && ( ((1*aFifth <= i) && (i < 2*aFifth)) || ((3*aFifth <= i) && (i < 4*aFifth))))
                return 0;
            break;
        case IN_EIGHT:
            int aQuater = width / 4;
            if (((2*aQuater <= i) && (i < 3 * aQuater)) && (((aQuater <= j) && (j < 2*aQuater))))
                return 0;
            if (((aQuater <= i) && (i < 2 * aQuater)) && (((2*aQuater <= j) && (j < 3*aQuater))))
                return 0;
            break;
        default:
            return (vertex < width*width);
    }
    return 1;
}

struct graph_t *create_graph(unsigned int width, enum graph_type shape)
{
    /*
    if ((shape != SQUARE) && (width < 5)) {
        printf("invalide width (must be > 5)\n");
        exit(EXIT_FAILURE);
    }*/
    
    struct graph_t *graph = malloc(sizeof(struct graph_t));
    gsl_spmatrix_uint *mat = gsl_spmatrix_uint_alloc(width * width, width * width);

    switch (shape) {
    case SQUARE:
        graph->num_vertices = width * width;
        break;
    
    case DONUT:
        if (width % 3 != 0) 
            width -= width % 3;
        graph->num_vertices = width * width * 8 / 9;
        break;

    case CLOVER:
        if (width % 5 != 0)
            width -= width % 5;
        graph->num_vertices = width * width * 21 / 25;
        break;

    case IN_EIGHT:
        if (width % 4 != 0)
            width -= width % 4;
        graph->num_vertices = width * width * 7 / 8;
        break;

    case EMPTY:
        graph->num_vertices = width * width;
        break;

    default:
        fprintf(stderr, "Given graph shape is not handled in creation : \"%c\" \n", shape);
        break;
    }


    for (size_t vertex = 0; (vertex < width * width); vertex++)
    {
        if (!is_vertex_in_shape(width,shape,vertex))
            continue;
        for (enum dir_t dir = FIRST_DIR; dir <= LAST_DIR; dir++)
        {
            size_t neighbor = get_neighbor(width, vertex, dir);
            if (neighbor == UINT_MAX || !is_vertex_in_shape(width,shape,neighbor))
                continue;
            gsl_spmatrix_uint_set(mat, vertex, neighbor, dir);
        }
    }

    // converting the matrix to the CSR format
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

void print_graph(const struct graph_t *g)
{
    printf("num_vertices : %u\n", g->num_vertices);
    gsl_spmatrix_uint_fprintf(stdout, g->t, "%u");

    printf("\n");
    for (size_t i = 0; i < g->num_vertices; i++)
    {
        for (size_t j = 0; j < g->num_vertices; j++)
            printf("%u ", gsl_spmatrix_uint_get(g->t, i, j));
        printf("\n");
    }
}

void print_vertex(unsigned int width, enum graph_type shape) {
    for (size_t vertex = 0; vertex < width*width; vertex++)
    {
        if (vertex%width == 0)
            printf("\n");
        

        if (is_vertex_in_shape(width,shape,vertex)) 
            printf("X ");
        else 
            printf("  ");
    }
    printf("\n");
    
}

void add_edge(struct graph_t *g, size_t v1, size_t v2, unsigned int value)
{
    if (strcmp(gsl_spmatrix_uint_type(g->t), "CSR") == 0)
    {
        // unsigned int size = (unsigned int) sqrt(g->num_vertices);

        gsl_spmatrix_uint *uncompress = gsl_spmatrix_uint_compress(g->t, GSL_SPMATRIX_COO);

        if (gsl_spmatrix_uint_set(uncompress, v1, v2, value))
        {
            fprintf(stderr, "Failed to add an edge with value %u from vertex %lu to %lu\n", value, v1, v2);
            exit(EXIT_FAILURE);
        }
        gsl_spmatrix_uint_csr(g->t, uncompress);
    }
}

void remove_edge(struct graph_t *g, size_t v1, size_t v2)
{
    unsigned int *ptr = gsl_spmatrix_uint_ptr(g->t, v1, v2);
    *ptr = NO_DIR;
}

bool exist_edge_value(const struct graph_t *g, size_t v1, size_t v2, unsigned int value)
{
    return (gsl_spmatrix_uint_get(g->t, v1, v2) == value);
}

bool exist_edge(const struct graph_t *g, size_t v1, size_t v2)
{
    return (gsl_spmatrix_uint_get(g->t, v1, v2) != NO_DIR);
}

enum graph_type convert_char_to_shape(char shape)
{
    switch (shape)
    {
    case 's':
        return SQUARE;
    case '8':
        return IN_EIGHT;
    case 'd':
        return DONUT;
    case 'c':
        return CLOVER;
    case 'e':
        return EMPTY;
    default:
        return SHAPE_ERROR;
    }
}

struct graph_t *graph_copy(const struct graph_t *g)
{

    struct graph_t *copy_graph = create_graph(get_width(g), EMPTY);

    if (gsl_spmatrix_uint_memcpy(copy_graph->t, g->t) != 0)
    {
        fprintf(stderr, "Failed to copy matrix");
        exit(EXIT_FAILURE);
    }
    return copy_graph;
}
