#include "test.h"
#include "queens.h"

int test_queens_compute_number(void){
    return 
    !(queens_compute_number(5) == 4 &&
      queens_compute_number(8) == 4 &&
      queens_compute_number(12) == 8 &&
      queens_compute_number(50) == 24);
}

static int contain_the_same_elements(int * array1, int * array2, int size){
    int is_in_table = 1;
    for(int i = 0; i < size; i++){
        is_in_table = 0;
        for(int j = 0; j < size; j++){
            if(array1[i] == array2[j]){
                is_in_table = 1;
            }
        }
        if(!is_in_table){
                return 0;
        }
        else{
            is_in_table = 0;
        }
    }
    return 1;
}
static void print_queen(int* array, int size, int k){
    for(int i = 0; i < size; i++){
        if(array[i] == k){
            printf(" 8 ");
            return;
        }
    }
    printf(" | ");
}
static void print_game(int* array, int size, int m){
    printf("\n ");
    printf("\n ");
    for(int i = 0; i < m; i++){
        for(int j = 0; j < m; j++){
            print_queen(array, size, i*m+j);
        }
        printf("\n ");
    }
    printf("\n ");
    printf("\n ");    
}
int test_queens_create_positions(void){
    int from = 5;
    int to = 5;
    unsigned int* positions_to_print;
    for(int i = from; i < to; i++){ // prints the games of size i twice with the queens of each player 
        positions_to_print = queens_create_positions(i,0);
        print_game(positions_to_print,queens_compute_number(i),i);
        queens_free_positions(positions_to_print);
        positions_to_print = queens_create_positions(i,1);
        print_game(positions_to_print,queens_compute_number(i),i);
        queens_free_positions(positions_to_print);
    }
    unsigned int* computed_positions_5 = queens_create_positions(5,0);
    int positions_5[4] = {1,3,5,9};
    int result_5 = contain_the_same_elements(computed_positions_5,positions_5,4);
    queens_free_positions(computed_positions_5);

    unsigned int* computed_positions_12 = queens_create_positions(12,1);
    int positions_12[8] = {142,140,133,135,131,120,96,107};
    int result_12 = contain_the_same_elements(computed_positions_12,positions_12,8);
    queens_free_positions(computed_positions_12);

    unsigned int* computed_positions_15 = queens_create_positions(15,0);
    int positions_15[8] = {2,5,9,12,30,75,44,89};
    int result_15 = contain_the_same_elements(computed_positions_15,positions_15,8);
    queens_free_positions(computed_positions_15);

    return !(result_12 && result_15 && result_5);
}

int test_queens_copy(void){

    return 1;
}