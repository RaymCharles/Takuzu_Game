#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "game_struct.h"
#include "game_aux.h"
#include "game_tools.h"

int main(int argc, char *argv[]) {
    // check the number of arguments (with or without the third parameter)
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "You have to put 2 or 3 arguments !\n");
        return EXIT_FAILURE;
    }
    // collects the option and the files' name put in parameters
    char *option = argv[1];
    char *input = argv[2];
    // if the user didn't specify the name of the output file
    char *output = NULL;
    // if the user specified the name of the output file
    if (argc == 4){
        output = argv[3];
    }
    // loading the game desired by the user
    game g = game_load(input);
    assert(g != NULL);

    // the user wishes to search for a solution
    if (strcmp(option, "-s") == 0) {
        bool result = game_solve(g);
        // no solution found
        if (result == false) {
            fprintf(stderr, "No solution found...\n");
            free(g);
            return EXIT_FAILURE;
        }
        // solution found and the user didn't specify the name of the output file
        if (output == NULL) {
            printf("Here is a solution found :\n");
            game_print(g);
        }
        // solution found and the user specified the name of the output file
        else {
            FILE *output_file = fopen(output, "w");
            assert(output_file != NULL);
            // cas particuliers pour séparer si une ou plusieurs solutions ou pas utile ?
            game_save(g, output);
            fclose(output_file);
        }
    }
    // the user wants to count the total number of possible solutions
    else if (strcmp(option, "-c") == 0) {
        int nb_solutions = game_nb_solutions(g);
        // the user didn't specify the name of the output file
        if (output == NULL) {
            printf("Number of solution(s) : %d\n", nb_solutions);
        }
        // the user specified the name of the output file
        else {
            FILE *output_file = fopen(output, "w");
            assert(output_file != NULL);
            fprintf(output_file, "%d\n", nb_solutions);
            fclose(output_file);
        }
    }
    else {
        fprintf(stderr, "Invalid option !\n");
        free(g);
        return EXIT_FAILURE;
    }
    free(g);
    return EXIT_SUCCESS;
}