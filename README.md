# Subject
The subject is available [here](https://www.labri.fr/perso/renault/working/teaching/projets/2022-23-S6-C-Amazons.php).
This project was made by Tanguy Daponte, Abdurahman El Califa Kan It Bensaidi, Maximilien Vidiani and Louis Peyrondet over a 1.5-month period for the 1st year programming project at the Enseirb-Matmeca school. 

# Compilation

The only dependency is the GSL library. The path can be passed as the "GSL_PATH" variable.

    make && make install

To get rif of the debug flag and add the -O3 optimisation flag pass "TURBO=true" as a variable like so

    TURBO=true make && make install


To launch a standard game 

    make play

To compile the arena

    make arena

## Sources files


| Source file | Description |
| ----------- | ----------- |
| server | Server executable. Loads the commmand line arguments and launches a game with them |
| game | Main loop of the game. Dynamically loads the clients libraries |
| board | Abstraction layer between the graph structure to only give a board representation in its API |
| graph | Abstraction layer for the GSL sparse matrix library |
| heuristic | Implementation of different heuristics for the board |
| move_logic | Functions for the logic of the Amazons games such as getting all the possible moves for a queen on the board |
| queens | Handles the queens creation and states |
| territories | Compute the "territories" of a game board. Used by the heuristics |
| queue | Queue data structure implementation |
| tree | Tree data structure implementation |
| arena | A special server to test the clients against each other for performances and win rates data |
| [\*]client[\*] | Implementation of a client. Each client differs in the heuristics and strategies used |
| montecarlo | A client using a montecarlo tree search |
