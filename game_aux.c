#define __USE_GNU

// HEADER FILES

#include "game_aux.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_struct.h"

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_PRINT  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Prints a game as text on the standard output stream.
                The different squares are respectively displayed as text.
---------------------------------------------------------------------------------------------
*/

void game_print(cgame g) {
  if (g == NULL) {
    // g is not a valid pointer toward a game structure
    fprintf(stderr, "Invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }

  printf("   ");
  for (uint x = 0; x < g->size_column; x++) {
    // display of column numbers
    printf("%d", x);
  }
  printf("\n");
  printf("   ");
  for (uint x = 0; x < g->size_column; x++) {
    printf("-");
  }
  printf("\n");
  for (uint x = 0; x < g->size_line; x++) {
    // display of line numbers
    printf("%d |", x);
    for (uint y = 0; y < g->size_column; y++) {
      // display of the value of each square
      if (game_get_number(g, x, y) == -1) {
        // empty square
        printf(" ");
      }
      if (game_get_number(g, x, y) == 1 && game_is_immutable(g, x, y) == true) {
        // immutable Black square
        printf("B");
      }
      if (game_get_number(g, x, y) == 0 && game_is_immutable(g, x, y) == true) {
        // immutable White square
        printf("W");
      }
      if (game_get_number(g, x, y) == 1 &&
          game_is_immutable(g, x, y) == false) {
        // black square
        printf("b");
      }
      if (game_get_number(g, x, y) == 0 &&
          game_is_immutable(g, x, y) == false) {
        // white square
        printf("w");
      }
    }
    printf("|\n");
  }
  printf("   ");
  for (uint x = 0; x < g->size_column; x++) {
    printf("-");
  }
  printf("\n");
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_DEFAULT  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Creates the default game.
  RETURN : Returns the created game.
---------------------------------------------------------------------------------------------
*/

game game_default(void) {
  // creation of an empty game
  game g = game_new_empty();
  // assignment of the values of immutable squares
  game_set_square(g, 0, 1, S_IMMUTABLE_ONE);
  game_set_square(g, 0, 2, S_IMMUTABLE_ZERO);
  game_set_square(g, 2, 1, S_IMMUTABLE_ZERO);
  game_set_square(g, 2, 4, S_IMMUTABLE_ZERO);
  game_set_square(g, 3, 1, S_IMMUTABLE_ZERO);
  game_set_square(g, 3, 2, S_IMMUTABLE_ONE);
  game_set_square(g, 4, 2, S_IMMUTABLE_ONE);
  game_set_square(g, 4, 5, S_IMMUTABLE_ZERO);
  game_set_square(g, 5, 5, S_IMMUTABLE_ZERO);

  return g;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_DEFAULT_SOLUTION  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Creates the default game solution.
  RETURN : Returns the created game.
---------------------------------------------------------------------------------------------
*/

game game_default_solution(void) {
  // creation of an empty game
  game g = game_new_empty();
  // values assignment of immutable squares and squares making it possible to
  // win
  game_set_square(g, 0, 0, S_ZERO);
  game_set_square(g, 0, 1, S_IMMUTABLE_ONE);
  game_set_square(g, 0, 2, S_IMMUTABLE_ZERO);
  game_set_square(g, 0, 3, S_ONE);
  game_set_square(g, 0, 4, S_ZERO);
  game_set_square(g, 0, 5, S_ONE);
  game_set_square(g, 1, 0, S_ZERO);
  game_set_square(g, 1, 1, S_ONE);
  game_set_square(g, 1, 2, S_ONE);
  game_set_square(g, 1, 3, S_ZERO);
  game_set_square(g, 1, 4, S_ONE);
  game_set_square(g, 1, 5, S_ZERO);
  game_set_square(g, 2, 0, S_ONE);
  game_set_square(g, 2, 1, S_IMMUTABLE_ZERO);
  game_set_square(g, 2, 2, S_ZERO);
  game_set_square(g, 2, 3, S_ONE);
  game_set_square(g, 2, 4, S_IMMUTABLE_ZERO);
  game_set_square(g, 2, 5, S_ONE);
  game_set_square(g, 3, 0, S_ONE);
  game_set_square(g, 3, 1, S_IMMUTABLE_ZERO);
  game_set_square(g, 3, 2, S_IMMUTABLE_ONE);
  game_set_square(g, 3, 3, S_ZERO);
  game_set_square(g, 3, 4, S_ZERO);
  game_set_square(g, 3, 5, S_ONE);
  game_set_square(g, 4, 0, S_ZERO);
  game_set_square(g, 4, 1, S_ONE);
  game_set_square(g, 4, 2, S_IMMUTABLE_ONE);
  game_set_square(g, 4, 3, S_ZERO);
  game_set_square(g, 4, 4, S_ONE);
  game_set_square(g, 4, 5, S_IMMUTABLE_ZERO);
  game_set_square(g, 5, 0, S_ONE);
  game_set_square(g, 5, 1, S_ZERO);
  game_set_square(g, 5, 2, S_ZERO);
  game_set_square(g, 5, 3, S_ONE);
  game_set_square(g, 5, 4, S_ONE);
  game_set_square(g, 5, 5, S_IMMUTABLE_ZERO);

  return g;
}
