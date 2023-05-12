#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <dlfcn.h>
#include <sys/time.h>

#include "game.h"
#include "player.h"

#define TEST_BOARDS 5

long get_current_time_microseconds(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long time = tv.tv_sec * 1000000 + tv.tv_usec;
    return time;
}

const char * shape_string(char s){
    if (s == 's') return "Square";
    else if (s == '8') return "In-eight";
    else if (s == 'd') return "Donut";
    else if (s == 'c') return "Clover";
    return "Unknown";
}

unsigned int index_max(unsigned int *arr, unsigned int count){
    unsigned int max = 0;
    for (size_t i = 0; i < count; i++)
    {
        if (arr[i] > arr[max]) max = i;  
    }
    return max;   
}

int main(int argc, char *argv[])
{
    int opt;
    long seed = get_current_time_microseconds();
    char *player1_path = NULL;
    char *player2_path = NULL;
    size_t rounds = 50;
    size_t clients_count = 2;
    while ((opt = getopt(argc, argv, "s:r:")) != -1)
    {
        switch (opt)
        {
        case 's':
            seed = atoi(optarg);
            break;
        case 'r':
            rounds = atoi(optarg);
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
        fprintf(stderr, "Usage: %s -s [S] -r [R] player1_path player2_path\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    char board_shapes[TEST_BOARDS] = {'s', 's',  'd', 'c', '8'};
    char board_sizes[TEST_BOARDS] = {5, 10, 15, 15 ,15};
    char *libraries_paths[NUM_PLAYERS] = {player1_path, player2_path}; 

    srand(seed);
    printf("Seed : %ld\n", seed);

    for (size_t i = 0; i < TEST_BOARDS; i++)
    {
        unsigned int board_size = board_sizes[i];
        char board_shape = board_shapes[i];
        unsigned int *wins = calloc(clients_count, sizeof(*wins));

        printf("On board %s size %d,%d :\n", shape_string(board_shape), board_size, board_size);
        double times[NUM_PLAYERS] = {0.0, 0.0};

        for (size_t j = 0; j < rounds; j++)
        {
            //if(j % 2){
                int winner = play_game(libraries_paths, board_size, board_shape, 0, times);
                wins[winner] += 1;
                /*
            }
            else{
                int winner = play_game(libraries_paths_reversed, board_size, board_shape, 0, times);
                wins[!winner] += 1;
            }*/
        }

        printf("\tWinner is Player %u\n", index_max(wins, clients_count));


        printf("\tDetails :\n");
        for (size_t j = 0; j < clients_count; j++)
        {
            printf("\t\tPlayer %zu \t: wins: %u\t avg game time: %lf\n", j, wins[j], times[j]/rounds);
        }
        free(wins);
    }


    return EXIT_SUCCESS;
}