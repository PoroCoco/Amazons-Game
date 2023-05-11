#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <assert.h>
#include <time.h>

#include "player.h"
#include "board.h"
#include "dir.h"
#include "game.h"
#include "graph_ext.h"
#include "queens.h"
#include "move_logic.h"
#include "client_explo.h"

typedef struct client_server {
    unsigned int id;
    char const* (*get_player_name)(void);
    void (*initialize)(unsigned int player_id, struct graph_t* graph,
                unsigned int num_queens, unsigned int* queens[NUM_PLAYERS]);
    struct move_t (*play)(struct move_t previous_move);
    void (*finalize)(void);
    void * handle;
} client_server_t;


void display_client(client_server_t *clients){
    for (size_t i = 0; i < NUM_PLAYERS; i++)
    {
        printf("I'm client id %u\n", clients[i].id);
    }
}


int play_game(char ** libraries_paths, unsigned int board_size, char board_type, int verbose, double *times){

    //determines the nb of queens 
    unsigned int queen_number = queens_compute_number(board_size);

    //creates the NUM_PLAYER queens arrays of sizes "queen_number" with the index of the position of each queen for each player.
    unsigned int *queens[NUM_PLAYERS]; 
    for (size_t i = 0; i < NUM_PLAYERS; i++)
    {
        queens[i] = queens_create_positions(board_size, i);
        assert(queens[i] != NULL);
    }


    //creates each client struc (open, load funcs)
    client_server_t clients[1];
    for (unsigned int i = 0; i < 1; i++)
    {
        clients[i] = load_client(1, libraries_paths[i]);
    }

    // display_client(clients);

    //create game_board
    struct graph_t *g = create_graph(board_size, convert_char_to_shape(board_type));
    board_t *game_board = board_create(g, queens, queen_number);

    //init each client
    for (unsigned int i = 0; i < 1; i++)
    {
        struct graph_t *graph_cpy = graph_copy(g); //malloc copy
        unsigned int *queens_cpy[NUM_PLAYERS];
        for (unsigned int i = 0; i < NUM_PLAYERS; i++)
        {
            queens_cpy[i] = queens_copy(queens[i], board_size); //malloc copy
            assert(queens_cpy[i] != NULL);
        }
        
        clients[i].initialize(clients[i].id, graph_cpy, queen_number, queens_cpy);
    }

        struct graph_t *graph_cpy = graph_copy(g); //malloc copy
        unsigned int *queens_cpy[NUM_PLAYERS];
        for (unsigned int i = 0; i < NUM_PLAYERS; i++)
        {
            queens_cpy[i] = queens_copy(queens[i], board_size); //malloc copy
            assert(queens_cpy[i] != NULL);
        }
        
        client_explo_t *c = initialize_explo(0, graph_cpy, queen_number, queens_cpy);
    printf("%s\n", get_player_name_explo(c));


    // for (size_t i = 0; i < game_board->board_cells; i++)
    // {
    //     for (size_t j = 0; j < game_board->board_cells; j++)
    //     {
    //         printf("%d", get_move_direction(game_board, i, j));
    //     }
    //     printf("\n");
    // }
    

    //game loop
    clock_t start_time, end_time;
    struct move_t m = {-1, -1, -1};
    size_t max_turns = game_board->board_cells;
    unsigned int current_player = 1;
    for (size_t i = 0; i < max_turns; i++)
    {
        if (verbose > 1) board_print(game_board);

        //checks if the current player can move
        if(is_game_over_for_player(game_board, current_player)){
            break;
        }

  
        start_time = clock();
        if (current_player == clients[0].id){
            // printf("calling play client\n");
            m = clients[0].play(m);
        }else{
            // printf("calling play explo\n");
            m = play_explo(c, m);
        }

        end_time = clock();
        
        // printf("Playing turn took %lf for player named %s id %u:\n", (double)(end_time - start_time) / CLOCKS_PER_SEC, clients[current_player].get_player_name(), current_player);
        times[current_player] += (double)(end_time - start_time) / CLOCKS_PER_SEC;

        //check move valid
        if (!is_move_valid(game_board, &m, current_player)){
            if (verbose > 0) printf("Player %u %s gave an invalid move!\n", current_player, clients[current_player].get_player_name());
            if (verbose > 0) printf("{arrow_dst : %u, queen_src : %u, queen_dst : %u}\n", m.arrow_dst,m.queen_src, m.queen_dst);
            break;
        }

        apply_move(game_board, &m, current_player);
        current_player++;
        if (current_player >= NUM_PLAYERS){
            current_player = 0;
        }
    }
    

    //finalize each client
    for (unsigned int i = 0; i < 1; i++)
    {
        clients[i].finalize();
        dlclose(clients[i].handle);
    }
    finalize_explo(c);
    //free game_board
    board_free(game_board);

    return 1 - current_player;
}

struct client_server load_client(unsigned int id, char * library_path){
    assert(library_path);
    
    struct client_server c;
    c.id = id;

    char *error;
    c.handle = dlopen(library_path, RTLD_LAZY);
    if (!c.handle) {
        fputs (dlerror(), stderr);
        exit(EXIT_FAILURE);
    }

    c.get_player_name = dlsym(c.handle, "get_player_name");
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        exit(EXIT_FAILURE);
    }

    c.initialize = dlsym(c.handle, "initialize");
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        exit(EXIT_FAILURE);
    }

    c.play = dlsym(c.handle, "play");
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        exit(EXIT_FAILURE);
    }

    c.finalize = dlsym(c.handle, "finalize");
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr);
        exit(EXIT_FAILURE);
    }

    return c;
}