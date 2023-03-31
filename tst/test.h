
#ifndef __TEST_PROJECT_H__
#define __TEST_PROJECT_H__

#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<string.h>

/* ************************************************************************** */
/*                                test graph                                  */
/* ************************************************************************** */

int test_square_graph(void);

int test_get_move_direction(void);



/* ************************************************************************** */
/*                                test queens                                 */
/* ************************************************************************** */
int test_queens_compute_number(void);

int test_queens_create_positions(void);

int test_queens_copy(void);

#endif