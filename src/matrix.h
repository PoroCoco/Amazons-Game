#ifndef _AMAZON_MATRIX_H
#define _AMAZON_MATRIX_H

/// @brief Enum specifiyng different kinds of graph
enum graph_type{
    SQUARE,
    DONUT,
    CLOVER,
    IN_EIGHT
};

/// @brief Creates a graph of the given shape on the heap and returns a pointer on it. 
/// @param size The size of the graph (size*size)
/// @param shape The shape of the graph
struct graph_t *create_graph(unsigned int size, enum graph_type shape);

/// @brief Updates the graph by adding an edge from v1 to v2. If the edge already exists this does nothing.
/// @param g The graph to update
/// @param v1 The starting vertex 
/// @param v2 The ending vertex
void add_edge(struct graph_t *g, unsigned int v1, unsigned int v2);


/// @brief Updates the graph by remove the edge from v1 to v2. If the edge is already removed does nothing.
/// @param g The graph to update
/// @param v1 The starting vertex 
/// @param v2 The ending vertex
void remove_edge(struct graph_t *g, unsigned int v1, unsigned int v2);


/// @brief Frees the graph given
/// @param g The graph to free
void destroy_graph(struct graph_t *g);







#endif // _AMAZON_MATRIX_H
