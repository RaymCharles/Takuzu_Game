/* required for implementation */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* include & check declaration */
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"
/* auxiliary functions... */

/* This function returns a message to help the gamer */
void help() {
  printf("\naction : help\n");
  printf("- press 'w <i> <j>' to put a zero/white at square (i,j)\n");
  printf("- press 'b <i> <j>' to put a one/black at square (i,j)\n");
  printf("- press 'e <i> <j>' to empty square (i,j)\n");
  printf("- press 'r' to restart\n");
  printf("- press 'q' to quit\n");
  printf("- press 'z' to undo a move\n");
  printf("- press 'y' to redo a move\n\n");
}

/* This function prints the list of errors */
int error(cgame g) {
  for (uint i = 0; i < game_nb_rows(g); i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
      int e = game_has_error(g, i, j);
      if (e != 0) {
        printf("The square line %d column %d has an error : %d\n", i, j, e);
      }
    }
  }
  return EXIT_SUCCESS;
}

/* This function returns the value of the command desired by the gamer */
int value(char command) {
  if (command == 'w') {
    return S_ZERO;
  } else if (command == 'b') {
    return S_ONE;
  } else if (command == 'e') {
    return S_EMPTY;
  }
  return -1;  // the function returns -1 if the command is not equal to 'w', 'b'
              // or 'e' even if this situation will not exist thanks to the
              // condition before the call
}

/* main function */
// int main(void) {
// game g = game_new_ext(4, 8, true, true);

int main(int argc, char* argv[]) {
  game g;
  if (argc > 2) {
    return EXIT_FAILURE;

  } else if
    (argc ==2){
      g=game_load(argv[1]);
    }else{
      g=game_default();
    }    

  // game g = game_default();
  int r1, r2;
  while (!game_is_over(g)) {
    game_print(g);
    error(g);
    char cmd;
    printf("Please, enter a character : \n");
    r1 = scanf(" %c", &cmd);

    if ((r1 == 1) & (cmd == 'h')) {  // The user needs help.
      help();

    } else if ((r1 == 1) &
               (cmd == 'r')) {  // The user wants to restart the game.
      game_restart(g);

    } else if ((r1 == 1) & (cmd == 'q')) {  // The user wants to quit the game.
      printf("shame\n");
      return EXIT_SUCCESS;

    } else if ((r1 == 1) & (cmd == 'z')) {  // The user wants to undo the game.
      printf("undo last move\n");
      game_undo(g);

    } else if ((r1 == 1) & (cmd == 'y')) {  // The user wants to redo the game.
      printf("redo the canceled move\n");
      game_redo(g);

    } else if ((r1 == 1) & (cmd == 's')) {
      char file_save[200];
      r2 = scanf(" %s", file_save);
      game_save(g, file_save);
    }

    else if ((r1 == 1) & (cmd == 'w' || cmd == 'b' || cmd == 'e')) {
      int i;
      int j;
      printf("Please, enter a move : \n");
      r2 = scanf(" %u %u", &i, &j);
      if ((r2 == 2) & (game_check_move(g, i, j, value(cmd)))) {
        if (cmd == 'w') {
          game_play_move(g, i, j, S_ZERO);
        } else if (cmd == 'b') {
          game_play_move(g, i, j, S_ONE);
        } else if (cmd == 'e') {
          game_play_move(g, i, j, S_EMPTY);
        }
      }
    }
  }

  game_print(g);
  printf("congratulation\n");
  game_delete(g);
  return EXIT_SUCCESS;
}
