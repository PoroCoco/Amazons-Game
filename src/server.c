#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <dlfcn.h>

#include "game.h"
#include "player.h"

long get_current_time_microseconds(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long time = tv.tv_sec * 1000000 + tv.tv_usec;
    return time;
}

int main(int argc, char *argv[])
{
    int opt;
    int board_size = 8;
    srand(get_current_time_microseconds());
    char board_shape = 's';
    char *player1_path = NULL;
    char *player2_path = NULL;
    while ((opt = getopt(argc, argv, "t:m:s:")) != -1)
    {
        switch (opt)
        {
        case 'm':
            board_size = atoi(optarg);
            break;
        case 't':
            board_shape = optarg[0];
            break;
        case 's':
            srand(atoi(optarg));
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
    char *libraries_paths[NUM_PLAYERS] = {player1_path, player2_path}; 

    play_game(libraries_paths, board_size, board_shape);

    return EXIT_SUCCESS;
}