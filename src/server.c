#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <dlfcn.h>

int main(int argc, char *argv[])
{

    const char *(*get_name)(void);
    void *client_1;

    client_1 = dlopen("./install/client_1.so", RTLD_LAZY);
    get_name = dlsym(client_1, "get_player_name");

    int opt;
    int board_size = 8;
    char board_shape = 'c';
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

    printf("Un plateau de forme %c et de taille %d \n", board_shape, board_size);
    printf("Nom du client 1 : %s\n", (*get_name)());
    dlclose(client_1);

    return EXIT_SUCCESS;
}