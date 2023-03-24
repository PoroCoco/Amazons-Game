#include"test_project.h"

#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define NRM  "\x1B[0m"

void print_result(int result, char *test){
    if(result == 0){
        printf("%sTesting %s... ->%s PASSED\n",NRM,test,GRN);
    }
    else if (result != -1){
        fprintf(stderr,"%sTesting %s... ->%s NOT PASSED\n",NRM,test,RED);
    }
    else{
        fprintf(stderr,"Invalid argument!%d\n",result);
    }
}

struct test {
  char* name;
  int (*function)(void);
};

struct test tests[] = {
    {test_dummy,"dummy"}
    {0,0}
};


int main (int argc, char *argv[]){
    time_t t;
    srand((unsigned) time(&t));
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
