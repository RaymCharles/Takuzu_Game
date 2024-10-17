#define __USE_GNU

#include "game.h"


#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_ext.h"
#include "game_struct.h"

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_NEW  * * * * *
---------------------------------------------------------------------------------------------
*/

game game_new(square* squares) {
  assert(squares != NULL);
  // pointer g allocation - type "game" and size "sizeof(game_s)"
  game g = malloc(sizeof(game_s));
  assert(g != NULL);
  // history creation
  g->histo_played = queue_new();
  g->histo_canceled = queue_new();

  // values assignment of each parameter of g
  g->unique = false;
  g->wrapping = false;
  g->size_column = DEFAULT_SIZE;
  g->size_line = DEFAULT_SIZE;

  // pointer g->tab allocation - size "DEFAULT_SIZE * DEFAULT_SIZE *
  // sizeof(square)"
  g->tab = malloc(DEFAULT_SIZE * DEFAULT_SIZE * sizeof(square));
  if (g->tab == NULL) {
    // freeing allocated memory because the pointer g->tab is invalid
    queue_free(g->histo_canceled);
    queue_free(g->histo_played);
    free(g);
    fprintf(stderr, "Malloc problem : invalid pointer g->tab !\n");
    exit(EXIT_FAILURE);
  }
  // values assignment of each square
  for (uint i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    g->tab[i] = squares[i];
  }
  return g;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_NEW_EMPTY  * * * * *
---------------------------------------------------------------------------------------------
*/

game game_new_empty(void) {
  // pointer g allocation - type "game" and size "sizeof(game_s)"
  game g = malloc(sizeof(game_s));
  assert(g != NULL);
  // history creation
  g->histo_played = queue_new();
  g->histo_canceled = queue_new();
  // values assignment of each parameter of g
  g->wrapping = false;
  g->unique = false;
  g->size_column = DEFAULT_SIZE;
  g->size_line = DEFAULT_SIZE;

  // pointer s allocation - type "square*"
  // and size "(g->size_line) * (g->size_column) * sizeof(square)"
  square* s = malloc((g->size_line) * (g->size_column) * sizeof(square));
  if (s == NULL) {
    // freeing allocated memory because the pointer s is invalid
    queue_free(g->histo_canceled);
    queue_free(g->histo_played);
    free(g);
    fprintf(stderr, "Malloc problem : invalid pointer s !\n");
    exit(EXIT_FAILURE);
  }
  g->tab = s;
  for (uint x = 0; x < g->size_column; x++) {
    for (uint y = 0; y < g->size_line; y++) {
      // initialization of all squares with empty squares
      game_set_square(g, y, x, S_EMPTY);
    }
  }
  return g;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_COPY  * * * * *
---------------------------------------------------------------------------------------------
*/

game game_copy(cgame g) {
  assert(g != NULL);
  // creation of a game's copy
  game g_copy = game_new_ext(g->size_line, g->size_column, g->tab, g->wrapping,
                             g->unique);

  // values assignment of each parameter of g_copy
  g_copy->size_line = g->size_line;
  g_copy->size_column = g->size_column;
  g_copy->unique = g->unique;
  g_copy->wrapping = g->wrapping;

  // values assignment of each square
  for (uint i = 0; i < g->size_line * g->size_column; i++) {
    g_copy->tab[i] = g->tab[i];
  }
  return g_copy;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_EQUAL  * * * * *
---------------------------------------------------------------------------------------------
*/

bool game_equal(cgame g1, cgame g2) {
  assert(g1 != NULL && g2 != NULL);
  if (g1->size_column != g2->size_column || g1->size_line != g2->size_line) {
    // g1 and g2 don't have the same size
    return false;
  }
  if (game_is_unique(g1) != game_is_unique(g2) ||
      game_is_wrapping(g1) != game_is_wrapping(g2)) {
    // g1 and g2 don't have the same options
    return false;
  }
  for (uint j = 0; j < g1->size_column; j++) {
    for (uint i = 0; i < g1->size_line; i++) {
      if (game_get_square(g1, i, j) != game_get_square(g2, i, j)) {
        // g1 and g2 have a different square
        return false;
      }
    }
  }
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_DELETE  * * * * *
---------------------------------------------------------------------------------------------
*/

void game_delete(game g) {
  if (g == NULL) {
    // g is not a valid pointer toward a game structure
    fprintf(stderr, "Invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  if (g->tab == NULL) {
    // g->tab is not a valid pointer toward a square structure
    fprintf(stderr, "Invalid pointer g->tab !\n");
    exit(EXIT_FAILURE);
  }
  // deletion of the queue corresponding to played moves in the history
  while (g->histo_played != NULL && !queue_is_empty(g->histo_played)) {
    datamove* move = queue_pop_head(g->histo_played);
    free(move);
  }
  // deletion of the queue corresponding to canceled moves in the history
  while (g->histo_canceled != NULL && !queue_is_empty(g->histo_canceled)) {
    datamove* move = queue_pop_head(g->histo_canceled);
    free(move);
  }
  // freeing allocated memory of all pointers
  free(g->tab);
  queue_free(g->histo_canceled);
  queue_free(g->histo_played);
  free(g);
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_SET_SQUARE  * * * * *
---------------------------------------------------------------------------------------------
*/

void game_set_square(game g, uint i, uint j, square s) {
  if (g == NULL) {
    // g is not a valid pointer toward a game structure
    fprintf(stderr, "Invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  if (j < g->size_column && i < g->size_line) {
    // valid row index and valid column index :
    // value assignment of the square (i,j) corresponding to the value of the
    // square s
    g->tab[i * g->size_line + j] = s;
  }
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_GET_SQUARE  * * * * *
---------------------------------------------------------------------------------------------
*/

square game_get_square(cgame g, uint i, uint j) {
  assert(g != NULL);
  assert(g->tab != NULL);
  if (i < g->size_line && j < g->size_column) {
    // valid row index and valid column index :
    // finding value of the square (i,j)
    return (g->tab[i * g->size_column + j]);
  }
  // invalid row index or invalid column index
  fprintf(stderr, "Invalid row index or invalid column index !\n");
  exit(EXIT_FAILURE);
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_GET_NUMBER  * * * * *
---------------------------------------------------------------------------------------------
*/

int game_get_number(cgame g, uint i, uint j) {
  assert(g != NULL);
  assert(i <= g->size_line && j <= g->size_column);
  // value assignment -1 corresponding to an empty square
  if (game_get_square(g, i, j) == S_EMPTY) {
    return -1;
  }
  // value assignment 0 corresponding to a white square (immutable or not)
  if (game_get_square(g, i, j) == S_ZERO ||
      game_get_square(g, i, j) == S_IMMUTABLE_ZERO) {
    return 0;
  }
  // value assignment 1 corresponding to a black square (immutable or not)
  if (game_get_square(g, i, j) == S_ONE ||
      game_get_square(g, i, j) == S_IMMUTABLE_ONE) {
    return 1;
  }
  // square != S_EMPTY, S_ZERO, S_IMMUTABLE_ZERO, S_ONE, or S_IMMUTABLE_ONE
  fprintf(stderr, "Invalid square !\n");
  exit(EXIT_FAILURE);
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_GET_NEXT_SQUARE  * * * * *
---------------------------------------------------------------------------------------------
*/

int game_get_next_square(cgame g, uint i, uint j, direction dir, uint dist) {
  assert(g != NULL);
  assert(g->tab != NULL);
  assert(j < g->size_column && i < g->size_line);
  assert(dist <= 2);
  if (dir == UP) {
    // finding the value of a square by moving vertically upwards
    if (dist > i && !game_is_wrapping(g)) {
      // upwards overflow (game without wrapping option)
      return -1;
    }
    if (dist > i && game_is_wrapping(g)) {
      // no overflow (game with wrapping option)
      return game_get_square(g, i - dist + g->size_line, j);
    }
    // no overflow because the distance is <= index i
    return game_get_square(g, i - dist, j);
  }
  if (dir == DOWN) {
    // finding the value of a square by moving vertically downwards
    if (i + dist >= g->size_line && !game_is_wrapping(g)) {
      // downwards overflow (game without wrapping option)
      return -1;
    }
    if (i + dist >= g->size_line && game_is_wrapping(g)) {
      // no overflow (game with wrapping option)
      return game_get_square(g, i + dist - g->size_line, j);
    }
    // no overflow because the distance + index i is < game height
    return game_get_square(g, i + dist, j);
  }
  if (dir == LEFT) {
    // finding the value of a square by moving horizontally to the left
    if (dist > j && !game_is_wrapping(g)) {
      // overflow to the left (game without wrapping option)
      return -1;
    }
    if (dist > j && game_is_wrapping(g)) {
      // no overflow (game with wrapping option)
      return game_get_square(g, i, j - dist + g->size_column);
    }
    // no overflow because the distance is <= index j
    return game_get_square(g, i, j - dist);
  }
  if (dir == RIGHT) {
    // finding the value of a square by moving horizontally to the right
    if (j + dist >= g->size_column && !game_is_wrapping(g)) {
      // overflow to the right (game without wrapping option)
      return -1;
    }
    if (j + dist >= g->size_column && game_is_wrapping(g)) {
      // no overflow (game with wrapping option)
      return game_get_square(g, i, j + dist - g->size_column);
    }
    // no overflow because the distance + index j is < game width
    return game_get_square(g, i, j + dist);
  }
  // invalid direction
  fprintf(stderr, "Invalid direction !\n");
  exit(EXIT_FAILURE);
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_GET_NEXT_NUMBER  * * * * *
---------------------------------------------------------------------------------------------
*/

int game_get_next_number(cgame g, uint i, uint j, direction dir, uint dist) {
  assert(g != NULL);
  assert(g->tab != NULL);
  assert(dist <= 2);
  assert(j < g->size_column && i < g->size_line);
  if (dir == UP) {
    // finding the value of a square by moving vertically upwards
    if (dist > i && !game_is_wrapping(g)) {
      // upwards overflow (game without wrapping option)
      return -1;
    }
    if (dist > i && game_is_wrapping(g)) {
      // no overflow (game with wrapping option)
      return (game_get_number(g, i - dist + g->size_line, j));
    }
    // no overflow because the distance is <= index i
    return (game_get_number(g, i - dist, j));
  }
  if (dir == DOWN) {
    // finding the value of a square by moving vertically downwards
    if (i + dist >= g->size_line && !game_is_wrapping(g)) {
      // downwards overflow (game without wrapping option)
      return -1;
    }
    if (i + dist >= g->size_line && game_is_wrapping(g)) {
      // no overflow (game with wrapping option)
      return (game_get_number(g, i + dist - g->size_line, j));
    }
    // no overflow because the distance + index i is < game height
    return game_get_number(g, i + dist, j);
  }
  if (dir == LEFT) {
    // finding the value of a square by moving horizontally to the left
    if (dist > j && !game_is_wrapping(g)) {
      // overflow to the left (game without wrapping option)
      return -1;
    }
    if (dist > j && game_is_wrapping(g)) {
      // no overflow (game with wrapping option)
      return (game_get_number(g, i, j - dist + g->size_column));
    }
    // no overflow because the distance is <= index j
    return game_get_number(g, i, j - dist);
  }
  if (dir == RIGHT) {
    // finding the value of a square by moving horizontally to the right
    if (j + dist >= g->size_column && !game_is_wrapping(g)) {
      // overflow to the right (game without wrapping option)
      return -1;
    }
    if (j + dist >= g->size_column && game_is_wrapping(g)) {
      // no overflow (game with wrapping option)
      return (game_get_number(g, i, j + dist - g->size_column));
    }
    // no overflow because the distance + index j is < game width
    return game_get_number(g, i, j + dist);
  }
  // invalid direction
  fprintf(stderr, "Invalid direction !\n");
  exit(EXIT_FAILURE);
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_IS_EMPTY  * * * * *
---------------------------------------------------------------------------------------------
*/

bool game_is_empty(cgame g, uint i, uint j) {
  assert(g != NULL);
  assert(i <= g->size_line && j <= g->size_column);
  if (game_get_number(g, i, j) == -1) {
    // the square (i,j) is empty
    return true;
  }
  // the square (i,j) is not empty
  return false;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_IS_IMMUTABLE  * * * * *
---------------------------------------------------------------------------------------------
*/

bool game_is_immutable(cgame g, uint i, uint j) {
  assert(g != NULL);
  if (i < g->size_line && j < g->size_column) {
    // valid row index and valid column index :
    // test to determine if the square (i,j) is immutable
    square s = game_get_square(g, i, j);
    return (s == S_IMMUTABLE_ONE || s == S_IMMUTABLE_ZERO);
  }
  // invalid row index or invalid column index
  fprintf(stderr, "Invalid row index or invalid column index !\n");
  exit(EXIT_FAILURE);
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_HAS_ERROR  * * * * *
---------------------------------------------------------------------------------------------
*/

int game_has_error(cgame g, uint i, uint j) {
  assert(g != NULL);
  assert(i <= g->size_line && j <= g->size_column);
  assert(g->tab != NULL);
  if (game_get_square(g, i, j) == S_EMPTY) {
    // the square (i,j) is empty : no possible error
    return 0;
  }
  if (game_is_unique(g)) {
    // two rows must not be identical, nor two columns
    uint cpt;
    for (uint l = 0; l < g->size_column; l++) {
      // column check
      if (j != l) {
        // column l is another column than column j : we are going to compare
        // them
        cpt = 0;
        // this counter will correspond to the number of
        // identical squares of these two columns
        for (uint k = 0; k < g->size_line; k++) {
          // we look at the value of each row index in the column
          if (game_get_number(g, k, j) == game_get_number(g, k, l) &&
              game_get_square(g, k, j) != S_EMPTY) {
            // the column j have a square with the same value
            // as a square from an other column, for the same row index
            cpt++;
          }
        }
        if (cpt == g->size_line) {
          // the number of identical squares is equivalent to the size of a
          // column : a column is the same as column j
          return 1;
        }
      }
    }
    for (uint l = 0; l < g->size_line; l++) {
      // line check
      if (i != l) {
        // line l is another column than line i : we are going to compare them
        cpt = 0;
        // this counter will correspond to the number of
        // identical squares of these two lines
        for (uint k = 0; k < g->size_column; k++) {
          // we look at the value of each column index in the line
          if (game_get_number(g, i, k) == game_get_number(g, l, k) &&
              game_get_square(g, i, k) != S_EMPTY) {
            // the line i have a square with the same value
            // as a square from an other line, for the same column index
            cpt++;
          }
        }
        if (cpt == g->size_column) {
          // the number of identical squares is equivalent to the size of a line
          // a line is the same as line i
          return 1;
        }
      }
    }
  }
  if (game_get_number(g, i, j) == game_get_next_number(g, i, j, LEFT, 1) &&
      game_get_number(g, i, j) == game_get_next_number(g, i, j, LEFT, 2)) {
    // three consecutive squares have the same value (to the left)
    return 1;
  }
  if (game_get_number(g, i, j) == game_get_next_number(g, i, j, RIGHT, 1) &&
      game_get_number(g, i, j) == game_get_next_number(g, i, j, RIGHT, 2)) {
    // three consecutive squares have the same value (to the left)
    return 1;
  }
  if (game_get_number(g, i, j) == game_get_next_number(g, i, j, UP, 1) &&
      game_get_number(g, i, j) == game_get_next_number(g, i, j, UP, 2)) {
    // three consecutive squares have the same value (upwards)
    return 1;
  }
  if (game_get_number(g, i, j) == game_get_next_number(g, i, j, DOWN, 1) &&
      game_get_number(g, i, j) == game_get_next_number(g, i, j, DOWN, 2)) {
    // three consecutive squares have the same value (downwards)
    return 1;
  }
  if (game_get_number(g, i, j) == game_get_next_number(g, i, j, UP, 1) &&
      game_get_number(g, i, j) == game_get_next_number(g, i, j, DOWN, 1)) {
    // three consecutive squares have the same value (upwards and downwards)
    return 1;
  }
  if (game_get_number(g, i, j) == game_get_next_number(g, i, j, LEFT, 1) &&
      game_get_number(g, i, j) == game_get_next_number(g, i, j, RIGHT, 1)) {
    // three consecutive squares have the same value (to the left and to the
    // right)
    return 1;
  }
  uint counterZERO = 0;
  uint counterONE = 0;
  for (uint k = 0; k < g->size_column; k++) {
    if (game_get_number(g, i, k) == 0) {
      // white square
      counterZERO += 1;
    }
    if (game_get_number(g, i, k) == 1) {
      // black square
      counterONE += 1;
    }
  }
  if (counterZERO > (g->size_column) / 2 || counterONE > (g->size_column) / 2) {
    // there are too many white or black squares on the tested column
    return 1;
  }
  counterZERO = 0;
  counterONE = 0;
  for (uint k = 0; k < g->size_line; k++) {
    if (game_get_number(g, k, j) == 0) {
      // white square
      counterZERO += 1;
    }
    if (game_get_number(g, k, j) == 1) {
      // black square
      counterONE += 1;
    }
  }
  if (counterZERO > (g->size_line) / 2 || counterONE > (g->size_line) / 2) {
    // there are too many white or black squares on the tested line
    return 1;
  }
  return 0;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_CHECK_MOVE  * * * * *
---------------------------------------------------------------------------------------------
*/

bool game_check_move(cgame g, uint i, uint j, square s) {
  assert(g != NULL);
  if (i >= g->size_line) {
    // invalid row index : the coordinate i is not inside the grid
    fprintf(stderr, "Invalid row index !\n");
    return false;
  }
  if (j >= g->size_column) {
    // invalid column index : the coordinate j is not inside the grid
    fprintf(stderr, "Invalid column index !\n");
    return false;
  }
  if (s != S_ONE && s != S_ZERO && s != S_EMPTY) {
    // the square is either an empty, zero or one.
    fprintf(stderr, "Invalid square !\n");
    return false;
  }
  if (game_is_immutable(g, i, j)) {
    // the current square at (i,j) is an immutable square
    fprintf(stderr, "Immutable square !\n");
    return false;
  }
  // the move is legal
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_PLAY_MOVE  * * * * *
---------------------------------------------------------------------------------------------
*/

void game_play_move(game g, uint i, uint j, square s) {
  assert(g != NULL);
  assert(i <= g->size_line && j <= g->size_column);
  if (game_get_square(g, i, j) == S_IMMUTABLE_ONE ||
      game_get_square(g, i, j) == S_IMMUTABLE_ZERO) {
    // the square at position (i,j) is an immutable square
    fprintf(stderr,
            "The square at position (i,j) must not be an immutable square !\n");
    exit(EXIT_FAILURE);
  }
  if (s == S_IMMUTABLE_ZERO || s == S_IMMUTABLE_ONE) {
    // s must be either empty, zero or one, not an immutable square
    fprintf(stderr, "s must be either empty, zero or one !\n");
    exit(EXIT_FAILURE);
  }
  game_set_square(g, i, j, s);
  // pointer dm allocation - type "datamove*" and size "sizeof(datamove)"
  // dm variable is used to store information about the move played
  // (position (i, j) and value(s)) in a data structure called "datamove"
  datamove* dm = malloc(sizeof(datamove));
  if (dm == NULL) {
    // malloc problem : invalid pointer dm
    exit(EXIT_FAILURE);
  }
  // value assignment of each characteristic of the move
  dm->pos_i = i;
  dm->pos_j = j;
  dm->sq = s;
  if (g->histo_played == NULL || g->histo_canceled == NULL) {
    // invalid pointer history (played or canceled)
    fprintf(stderr, "Invalid pointer history (played or canceled) !\n");
    exit(EXIT_FAILURE);
  }
  // insertion of the played move in the history
  queue_push_head(g->histo_played, dm);
  while (!queue_is_empty(g->histo_canceled)) {
    // deletion of the history of canceled moves
    // by freeing the memory as long as the queue is not empty
    datamove* move = queue_pop_head(g->histo_canceled);
    free(move);
  }
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_IS_OVER  * * * * *
---------------------------------------------------------------------------------------------
*/

bool game_is_over(cgame g) {
  assert(g != NULL);
  assert(g->tab != NULL);
  for (uint j = 0; j < g->size_column; j++) {
    for (uint i = 0; i < g->size_line; i++) {
      if (game_has_error(g, i, j) != 0 || game_get_square(g, i, j) == S_EMPTY) {
        // the game has an error or the game still has an empty square
        return false;
      }
    }
  }
  // the game is over
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_RESTART  * * * * *
---------------------------------------------------------------------------------------------
*/

void game_restart(game g) {
  if (g == NULL) {
    // g is not a valid pointer toward a game structure
    fprintf(stderr, "Invalid pointer g !\n");
    exit(EXIT_FAILURE);
  }
  for (uint x = 0; x < g->size_column; x++) {
    for (uint y = 0; y < g->size_line; y++) {
      if ((game_get_square(g, y, x) != S_IMMUTABLE_ZERO) &&
          (game_get_square(g, y, x) != S_IMMUTABLE_ONE)) {
        // all the squares except immutable are reset to empty
        game_set_square(g, y, x, S_EMPTY);
      }
    }
  }
  while (g->histo_canceled != NULL && !queue_is_empty(g->histo_canceled)) {
    // deletion of the history of canceled moves
    // by freeing the memory as long as the queue is not empty
    datamove* move = queue_pop_head(g->histo_canceled);
    free(move);
  }
  while (g->histo_played != NULL && !queue_is_empty(g->histo_played)) {
    // deletion of the history of played moves
    // by freeing the memory as long as the queue is not empty
    datamove* move = queue_pop_head(g->histo_played);
    free(move);
  }
}