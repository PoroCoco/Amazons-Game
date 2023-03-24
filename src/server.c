#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>



int main(int argc, char * argv[]){
    int opt;
    int board_size = 8;
    char board_shape = 'c';
    while((opt = getopt(argc, argv, "s:m:")) != -1){
        switch(opt){
            case 'm':
                board_size = atoi (optarg);
            break;
            case 's':
                board_shape = optarg[0];
            break;
        }
    }

    printf("Un plateau de forme %c et de taille %d \n", board_shape, board_size);
    return EXIT_SUCCESS;
}