#include <getopt.h>
#include <dlfcn.h>

/// \cond
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/// \endcond

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
    long seed = get_current_time_microseconds();
    char board_shape = 's';
    char *player1_path = NULL;
    int verbose = 2;
    while ((opt = getopt(argc, argv, "t:m:s:v:")) != -1)
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
            seed = atoi(optarg);
            break;
        case 'v':
            verbose = atoi(optarg);
            break;
        }
    }

    if (optind < argc)
    {
        player1_path = argv[optind++];
    }
    if (!player1_path) {
        fprintf(stderr, "Usage: %s -m [M] -t [T] player1_path player2_path\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    srand(seed);
    printf("Board shape : %c and size : %d. Seed : %ld\n", board_shape, board_size, seed);
    printf("Path to first client %s\n",player1_path);
    char *libraries_paths[1] = {player1_path}; 
    double times[NUM_PLAYERS] = {0.0, 0.0}; 
    play_game(libraries_paths, board_size, board_shape, verbose, times);

    return EXIT_SUCCESS;
}