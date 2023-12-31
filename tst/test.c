#include "test.h"

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define NRM  "\x1B[0m"

void print_result(int result, char *test){
    if(result == 0){
        printf("%sTesting %s... ->%s PASSED%s\n",NRM,test,GRN,NRM);
    }
    else if (result != -1){
        fprintf(stderr,"%sTesting %s... ->%s NOT PASSED%s\n",NRM,test,RED,NRM);
    }
    else{
        fprintf(stderr,"Invalid argument!%d%s\n",result,NRM);
    }
}

struct test {
  char* name;
  int (*function)(void);
};

struct test tests[] = {
    {"create_and_free_square_graph", test_create_and_free_square_graph},
    {"different_graph_shape", test_different_graph_shapes},
    {"queens_compute_number", test_queens_compute_number},
    {"queens_create_positions", test_queens_create_positions},
    {"queens_copy", test_queens_copy},
    {"queens_occupy",test_queens_occupy},
    {"queens_move",test_queens_move},
    {"test_board_create", test_board_create},
    {"test_board_add_arrow", test_board_add_arrow},
    {"test_move_is_valid", test_is_move_valid},
    {"test_get_queen_liberty", test_get_queen_liberty},
    {"territory_array",test_territory_array},
    // {"queue",test_queue},
    {"test_node_create", test_node_create},
    {"test_node_add", test_node_add},
    {NULL,NULL}
};

int main (int argc, char *argv[]){
    if (argc == 1){
        for (struct test* pt = tests; pt->name && pt->function; pt++){
            print_result(pt->function(),pt->name);
        }
        return EXIT_SUCCESS;
    }
    else if(argc == 2){
        for (struct test* pt = tests; pt->name && pt->function; pt++){
            if(strcmp(argv[1], pt->name) == 0){
                print_result(pt->function(),pt->name);
                return EXIT_SUCCESS;
            }
        }
    }
    print_result(-1,NULL);
    return EXIT_SUCCESS;
}
