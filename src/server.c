#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <dlfcn.h>

int main(int argc, char *argv[])
{

    const char *(*get_name)(void);
    void *client_1;

    client_1 = dlopen("./build/client_1.so", RTLD_LAZY);
    get_name = dlsym(client_1, "get_player_name");

    int opt;
    int board_size = 8;
    char board_shape = 'c';
    char *player1_path = NULL;
    char *player2_path = NULL;
    while ((opt = getopt(argc, argv, "s:m:")) != -1)
    {
        switch (opt)
        {
        case 'm':
            board_size = atoi(optarg);
            break;
        case 's':
            board_shape = optarg[0];
            break;
        }
    }

    if (optind < argc)
    {
        player1_path = argv[optind++];
    }
    if (optind < argc) 
    {
        player2_path = argv[optind++];
    }
      if (!player1_path || !player2_path) {
        fprintf(stderr, "Usage: %s -m [M] -t [T] player1_path player2_path\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    printf("Board shape : %c and size : %d \n", board_shape, board_size);
    printf("Path to first client %s, path to second client %s \n",player1_path, player2_path);
    dlclose(client_1);

    return EXIT_SUCCESS;
}