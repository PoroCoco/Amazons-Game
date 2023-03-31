#ifndef _AMAZON_GRAPH_EXT_H
#define _AMAZON_GRAPH_EXT_H

#include "graph.h" 
#include "stdbool.h" 

/// @brief Enum specifiyng the different kinds of graph
enum graph_type{
    SQUARE,
    DONUT,
    CLOVER,
    IN_EIGHT,
    EMPTY,
};

/// @brief Creates a graph of the given shape on the heap and returns a pointer on it. The graph follows the CSR format.
/// @param size The size of the graph (size*size)
/// @param shape The shape of the graph
struct graph_t *create_graph(unsigned int size, enum graph_type shape);

/// @brief Updates the graph by adding an edge from v1 to v2. If the edge already exists this does nothing.
/// @param g The graph to update
/// @param v1 The starting vertex 
/// @param v2 The ending vertex
void add_edge(struct graph_t *g, size_t v1, size_t v2, unsigned int value);


/// @brief Updates the graph by removing the edge from v1 to v2. If the edge is already removed does nothing.
/// @param g The graph to update
/// @param v1 The starting vertex 
/// @param v2 The ending vertex
void remove_edge(struct graph_t *g, size_t v1, size_t v2);

/// @brief Returns boolean on if an edge between v1 and v2 exists.
/// @param g The graph to use
/// @param v1 The starting vertex
/// @param v2 The ending vertex
/// @return True if the edge exist, false otherwise 
bool exist_edge(const struct graph_t *g, size_t v1, size_t v2);

/// @brief Returns boolean on if an edge between v1 and v2 exists and has 'value' for value.
/// @param g The graph to use
/// @param v1 The starting vertex
/// @param v2 The ending vertex
/// @param value The value to match
/// @return True if the edge exist, false otherwise 
bool exist_edge_value(const struct graph_t *g, size_t v1, size_t v2, unsigned int value);

/// @brief Frees the given graph 
/// @param g The graph to free
void destroy_graph(struct graph_t *g);

/// @brief print the graph given
/// @param g The graph to print
void print_graph(const struct graph_t* g);

/// @brief Makes a heap allocated copy of g
/// @param g The graph to copy
struct graph_t* graph_copy(const struct graph_t* g);

/// @brief Converts the given character to the corresponding shape enum. If character doesn't match any, defaults to SQUARE
/// @param shape The character to convert
enum graph_type convert_char_to_shape(char shape);


#endif // _AMAZON_GRAPH_EXT_H
