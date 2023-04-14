
#ifndef __TEST_PROJECT_H__
#define __TEST_PROJECT_H__

#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<string.h>

/* ************************************************************************** */
/*                                test graph                                  */
/* ************************************************************************** */

int test_create_and_free_square_graph(void);

int test_different_graph_shapes(void);


/* ************************************************************************** */
/*                                test client                                 */
/* ************************************************************************** */

int test_get_move_direction(void);

int test_is_move_valid(void);

/* ************************************************************************** */
/*                                test board                                 */
/* ************************************************************************** */

int test_board_create(void);

int test_board_add_arrow(void);


/* ************************************************************************** */
/*                                test queens                                 */
/* ************************************************************************** */

int test_queens_compute_number(void);

int test_queens_create_positions(void);

int test_queens_copy(void);

int test_queens_occupy(void);

#endif