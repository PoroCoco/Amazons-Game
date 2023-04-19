#ifndef _AMAZON_GAME_H
#define _AMAZON_GAME_H

struct client;


/// @return The client id that won the game
int play_game(char ** libraries_paths, unsigned int board_size, char board_type);

/// @brief Creates the client struct and loads the functions for the given client library. The client struct should NOT be malloc'ed. Exit with error if the library path cannot be loaded.
/// @param id The id of the client
/// @param library_path The path to the dynamic library of the client
/// @return The filled client struct
struct client load_client(unsigned int id, char * library_path);

#endif // _AMAZON_GAME_H

