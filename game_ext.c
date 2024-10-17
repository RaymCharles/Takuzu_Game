#define __USE_GNU

// HEADER FILES

#include "game_ext.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_struct.h"

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_NEW_EXT  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Creates a new game with extended options and initializes it.
  RETURN : Returns the created game.
---------------------------------------------------------------------------------------------
*/

game game_new_ext(uint nb_rows, uint nb_cols, square *squares, bool wrapping,
                  bool unique) {
  if (squares == NULL) {
    // squares is not a valid pointer toward a square structure
    fprintf(stderr, "Invalid pointer squares !\n");
    exit(EXIT_FAILURE);
  }
  if (nb_cols % 2 == 1 || nb_rows % 2 == 1) {
    // problem : odd size array
    fprintf(stderr,
            "Invalid parameter nb_cols or nb_rows : odd size array !\n");
    exit(EXIT_FAILURE);
  }
  if (nb_cols < 2 || nb_rows < 2) {
    // problem : too small array
    fprintf(stderr,
            "Invalid parameter nb_cols or nb_rows : too small size array !\n");
    exit(EXIT_FAILURE);
  }
  /*
  if (nb_cols > 10 || nb_rows > 10) {
    // problem : too big array
    fprintf(stderr, "Invalid parameter nb_cols or nb_rows : too big array !\n");
    exit(EXIT_FAILURE);
  }
  */
  
  // pointer g allocation - type "game" and size "struct game_s"
  game g = malloc(sizeof(struct game_s));
  if (g == NULL) {
    // malloc problem : invalid pointer g
    fprintf(stderr, "Malloc problem : invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  // pointer g->tab allocation - type "square *" and size "nb_cols * nb_rows *
  // sizeof(square)"
  g->tab = (square *)malloc(nb_rows * nb_cols * sizeof(square));
  if (g->tab == NULL) {
    fprintf(stderr, "Malloc problem : invalid pointer g->tab !\n");
    // freeing allocated memory because the pointer g->tab is invalid
    free(g);
    exit(EXIT_FAILURE);
  }
  // test to verify that the squares have a valid value
  for (uint i = 0; i < nb_cols * nb_rows; i++) {
    if (squares[i] != S_ONE && squares[i] != S_ZERO && squares[i] != S_EMPTY &&
        squares[i] != S_IMMUTABLE_ONE && squares[i] != S_IMMUTABLE_ZERO) {
      exit(EXIT_FAILURE);
    }
    g->tab[i] = squares[i];
  }
  // creation of a new queue corresponding to the history of played moves
  g->histo_played = queue_new();
  /* A GARDER ?
      if (g->histo_played == NULL){
      free(g->tab);
      fprintf(stderr, "Invalid pointer !\n");
      free(g);
      exit(EXIT_FAILURE);
  }*/

  // creation of a new queue corresponding to the history of canceled moves
  g->histo_canceled = queue_new();
  /* A GARDER ?
      if (g->histo_canceled == NULL){
      free(g->tab);
      queue_free(g->histo_played);
      free(g);
      fprintf(stderr, "Invalid pointer !\n");
      exit(EXIT_FAILURE);
  }*/

  // values assignment of each parameter of g
  g->size_column = nb_cols;
  g->size_line = nb_rows;
  g->wrapping = wrapping;
  g->unique = unique;
  return g;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_NEW_EMPTY_EXT  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Creates a new empty game with extended options.
                All squares are initialized with empty squares.
  RETURN : Returns the created game.
---------------------------------------------------------------------------------------------
*/

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping,
                        bool unique) {
  if (nb_cols % 2 == 1 || nb_rows % 2 == 1) {
    // problem : odd size array
    fprintf(stderr,
            "Invalid parameters nb_cols or nb_rows : odd size array !\n");
    exit(EXIT_FAILURE);
  }
  if (nb_cols < 2 || nb_rows < 2) {
    // problem : too small array
    fprintf(stderr,
            "Invalid parameters nb_cols or nb_rows : too small array !\n");
    exit(EXIT_FAILURE);
  }
  /*
  if (nb_cols > 10 || nb_rows > 10) {
    // problem : too big array
    fprintf(stderr,
            "Invalid parameters nb_cols or nb_rows : too big array !\n");
    exit(EXIT_FAILURE);
  }
  */
  // pointer g allocation - type "game" and size "struct game_s"
  game g = malloc(sizeof(game_s));
  if (g == NULL) {
    // malloc problem : invalid pointer g
    fprintf(stderr, "Malloc problem : invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  // pointer g->tab allocation - type "square *" and size "nb_cols * nb_rows *
  // sizeof(square)"
  g->tab = (square *)malloc((nb_rows) * (nb_cols) * sizeof(square));
  if (g->tab == NULL) {
    // freeing allocated memory because the pointer g->tab is invalid
    free(g);
    fprintf(stderr, "Malloc problem : invalid pointer g->tab !\n");
    exit(EXIT_FAILURE);
  }
  for (uint x = 0; x < nb_cols * nb_rows; x++) {
    // Initialization of all squares with empty squares.
    g->tab[x] = S_EMPTY;
  }
  // creation of a new queue corresponding to the history of played moves
  g->histo_played = queue_new();
  /* A GARDER ?
    if (g->histo_played == NULL) {
    free(g);
    fprintf(stderr, "Invalid pointer !\n");
    exit(EXIT_FAILURE);
  }*/

  // creation of a new queue corresponding to the history of canceled moves
  g->histo_canceled = queue_new();
  /* A GARDER ?
    if (g->histo_canceled == NULL) {
    queue_free(g->histo_played);
    free(g);
    fprintf(stderr, "Invalid pointer !\n");
    exit(EXIT_FAILURE);
}*/

  // values assignment of each parameter of g
  g->size_column = nb_cols;
  g->size_line = nb_rows;
  g->wrapping = wrapping;
  g->unique = unique;

  return g;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_NB_ROWS  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Gets the number of rows (or height).
  RETURN : Returns the number of rows on this game.
---------------------------------------------------------------------------------------------
*/

uint game_nb_rows(cgame g) {
  if (g == NULL) {
    // g is not a valid pointer toward a cgame structure
    fprintf(stderr, "Invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  return g->size_line;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_NB_COLS  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Gets the number of columns (or width).
  RETURN : Returns the number of columns on this game.
---------------------------------------------------------------------------------------------
*/

uint game_nb_cols(cgame g) {
  if (g == NULL) {
    // g is not a valid pointer toward a cgame structure
    fprintf(stderr, "Invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  return g->size_column;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_IS_WRAPPING  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Checks if the game has the wrapping option
  RETURN : - true if wrapping option is enabled
           - false otherwise
---------------------------------------------------------------------------------------------
*/

bool game_is_wrapping(cgame g) {
  if (g == NULL) {
    // g is not a valid pointer toward a cgame structure
    fprintf(stderr, "Invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  return g->wrapping;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_IS_UNIQUE  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Checks if the game has the unique option.
  RETURN : - true if unique option is enabled
           - false otherwise
---------------------------------------------------------------------------------------------
*/

bool game_is_unique(cgame g) {
  if (g == NULL) {
    // g is not a valid pointer toward a cgame structure
    fprintf(stderr, "Invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  return g->unique;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_UNDO  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Undoes the last move.
                Searches in the history the last move played
                (by calling game_play_move or game_redo),
                and restores the state of the game before that move.
                If no moves have been played, this function does nothing.
                The game_restart function clears the history.
---------------------------------------------------------------------------------------------
*/

void game_undo(game g) {
  if (g == NULL) {
    // g is not a valid pointer toward a cgame structure
    fprintf(stderr, "Invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  // no played move
  if (queue_is_empty(g->histo_played)) {
    printf("You didn't play any move !\n");
    return;
  }
  // removing last played move
  datamove *dm = queue_pop_head(g->histo_played);
  game_set_square(g, dm->pos_i, dm->pos_j, S_EMPTY);
  queue_push_head(g->histo_canceled, dm);

  // seeking overlapping played move
  queue *tmp_histo_played = queue_new();
  while (!queue_is_empty(g->histo_played)) {
    datamove *tmp_dm = queue_pop_head(g->histo_played);
    queue_push_head(tmp_histo_played, tmp_dm);
    if (tmp_dm->pos_i == dm->pos_i && tmp_dm->pos_j == dm->pos_j) {
      game_set_square(g, dm->pos_i, dm->pos_j, tmp_dm->sq);
      break;
    }
  }
  // clearing tmp_histo_played and put back g->histo_played
  while (!queue_is_empty(tmp_histo_played)) {
    datamove *tmp_dm = queue_pop_head(tmp_histo_played);
    queue_push_head(g->histo_played, tmp_dm);
  }
  queue_free(tmp_histo_played);
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_REDO  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Redoes the last move.
                Searches in the history the last cancelled move
                (by calling game_undo), and replays it.
                If there are no more moves to be replayed, this function does
nothing. After playing a new move with game_play_move, it is no longer possible
to redo an old cancelled move.
---------------------------------------------------------------------------------------------
*/

void game_redo(game g) {
  if (g == NULL) {
    // g is not a valid pointer toward a cgame structure
    fprintf(stderr, "Invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  // no canceled move
  if (queue_is_empty(g->histo_canceled)) {
    printf("You didn't undo any move !\n");
    return;
  }
  datamove *dm = queue_pop_head(g->histo_canceled);
  game_set_square(g, dm->pos_i, dm->pos_j, dm->sq);
  queue_push_head(g->histo_played, dm);
}
