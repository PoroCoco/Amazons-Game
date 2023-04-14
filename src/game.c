#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <assert.h>

#include "player.h"
#include "board.h"
#include "dir.h"
#include "game.h"
#include "graph_ext.h"
#include "queens.h"
#include "move_logic.h"


typedef struct client {
    unsigned int id;
    char const* (*get_player_name)(void);
    void (*initialize)(unsigned int player_id, struct graph_t* graph,
                unsigned int num_queens, unsigned int* queens[NUM_PLAYERS]);
    struct move_t (*play)(struct move_t previous_move);
    void (*finalize)(void);
    void * handle;
} client_t;


void display_client(client_t *clients){
    for (size_t i = 0; i < NUM_PLAYERS; i++)
    {
        printf("I'm client id %u\n", clients[i].id);
    }
}


void play_game(char ** libraries_paths, unsigned int board_size, char board_type){

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
    client_t clients[NUM_PLAYERS];
    for (unsigned int i = 0; i < NUM_PLAYERS; i++)
    {
        clients[i] = load_client(i, libraries_paths[i]);
    }

    display_client(clients);

    //create game_board
    struct graph_t *g = create_graph(board_size, convert_char_to_shape(board_type));
    board_t *game_board = board_create(g, queens, queen_number);

    //init each client
    for (unsigned int i = 0; i < NUM_PLAYERS; i++)
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

    //game loop
    struct move_t m = {-1, -1, -1};
    size_t max_turns = 100;
    unsigned int current_player = 0;
    for (size_t i = 0; i < max_turns; i++)
    {
        board_print(game_board);

        //checks if the current player can move
        if(is_game_over_for_player(game_board, current_player)){
            printf("Player %u %s cannot move. Game is won by player %u %s!\n", current_player, clients[current_player].get_player_name(),  1 - current_player, clients[1 - current_player].get_player_name());
            break;
        }
        printf("Playing turn number %zu with player named %s id %u:\n", i, clients[current_player].get_player_name(), current_player);
        m = clients[current_player].play(m);
        
        //check move valid
        if (!is_move_valid(game_board, &m, current_player)){
            printf("Player %uu gave an invalid move!\n", current_player);
            break;
        }

        board_add_arrow(game_board, m.arrow_dst);
        queens_move(game_board->queens[current_player], game_board->board_width, m.queen_src, m.queen_dst);

        current_player++;
        if (current_player >= NUM_PLAYERS){
            current_player = 0;
        }
    }
    

    //finalize each client
    for (unsigned int i = 0; i < NUM_PLAYERS; i++)
    {
        clients[i].finalize();
        dlclose(clients[i].handle);
    }
    
    //free game_board
    board_free(game_board);
}

struct client load_client(unsigned int id, char * library_path){
    assert(library_path);
    
    struct client c;
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