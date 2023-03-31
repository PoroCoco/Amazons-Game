#include "test.h"

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define NRM  "\x1B[0m"

void print_result(int result, char *test){
    if(result == 0){
        printf("%sTesting %s... ->%s PASSED\n%s",NRM,test,GRN,NRM);
    }
    else if (result != -1){
        fprintf(stderr,"%sTesting %s... ->%s NOT PASSED\n%s",NRM,test,RED,NRM);
    }
    else{
        fprintf(stderr,"Invalid argument!%d\n%s",result,NRM);
    }
}

struct test {
  char* name;
  int (*function)(void);
};

struct test tests[] = {
    {"square_graph",test_square_graph},
    {"get_move_direction",test_get_move_direction},
    {"queens_compute_number",test_queens_compute_number},
    {"queens_create_positions",test_queens_create_positions},
    {"queens_copy",test_queens_copy},
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
