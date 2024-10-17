#define __USE_GNU
#include "game.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.c"

/*
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
  * * * * *  V1 TESTS  * * * * *
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
*/

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_NEW  * * * * *
---------------------------------------------------------------------------------------------
*/
bool test_game_new() {
  // definition of a new array of type "square"
  square tab[DEFAULT_SIZE];
  for (int k = 0; k < DEFAULT_SIZE; k++) {
    // initializing each square to an empty square
    tab[k] = S_EMPTY;
  }
  // creation of a new game
  game g = game_new(tab);
  if (g == NULL || sizeof(g) != sizeof(game)) {
    // invalid pointer g, failed creation of the new game
    return false;
  }
  // deleting pointers and freeing allocated memory
  game_delete(g);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_NEW_EMPTY  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_new_empty(void) {
  // creation of a new empty game
  game g = game_new_empty();
  if ((g == NULL) || (sizeof(g) != sizeof(game))) {
    // invalid pointer g, failed creation of the new empty game
    fprintf(stderr, "TEST ERROR : invalid pointer g !\n");
    return false;
  }
  for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
    for (unsigned int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != S_EMPTY) {
        // a non-empty square exists, failed creation of the new empty game
        fprintf(stderr, "TEST ERROR : a non-empty square exists !\n");
        // deleting pointers and freeing allocated memory
        game_delete(g);
        return false;
      }
    }
  }
  // deleting pointers and freeing allocated memory
  game_delete(g);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_COPY  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_copy(void) {
  // creation of a default game
  game g1 = game_default();
  // creation of an other game : the g1 game's copy
  game g2 = game_copy(g1);
  if (g1 == NULL || g2 == NULL) {
    // invalid pointer g1 or g2
    return false;
  }
  if (game_equal(g1, g2) == false) {
    // g1 and g2 are not the same, copy failed
    // deleting pointers and freeing allocated memory
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  // creation of a new empty game with the unique and wrapping options
  game g3 = game_new_empty_ext(6, 8, true, false);
  if (g3 == NULL) {
    // invalid pointer g3
    return false;
  }
  // value assignment of some squares
  game_set_square(g3, 0, 0, S_ZERO);
  game_set_square(g3, 1, 0, S_ONE);
  game_set_square(g3, 2, 0, S_ZERO);
  // creation of an other game : the g3 game's copy
  game g4 = game_copy(g3);
  if (g4 == NULL) {
    // invalid pointer g4, copy failed
    return false;
  }
  if (game_is_unique(g4) != game_is_unique(g3) ||
      // parameter unique is not the same for g4 and g3, copy failed
      game_is_wrapping(g4) != game_is_wrapping(g3) ||
      // parameter wrapping is not the same for g4 and g3, copy failed
      game_nb_rows(g4) != game_nb_rows(g3) ||
      //  g4 and g3 don't have the same rows number, copy failed
      game_nb_cols(g4) != game_nb_cols(g3)) {
    // g4 and g3 don't have the same cols number, copy failed

    // deleting pointers and freeing allocated memory
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    game_delete(g4);
    return false;
  }
  if (game_equal(g3, g4) == false) {
    // g3 and g4 are not the same, copy failed
    // deleting pointers and freeing allocated memory
    game_delete(g1);
    game_delete(g2);
    game_delete(g3);
    game_delete(g4);
    return false;
  }
  // deleting pointers and freeing allocated memory
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_EQUAL  * * * * *
---------------------------------------------------------------------------------------------
*/
bool test_game_equal() {
  // creation of two default games
  game g1 = game_default();
  game g2 = game_default();
  // creation of two default solution games
  game g3 = game_default_solution();
  game g5 = game_default_solution();
  // creation of two new empty games
  game g4 = game_new_empty();
  game g6 = game_new_empty();
  if (g1 == NULL || g2 == NULL || g3 == NULL || g4 == NULL || g5 == NULL ||
      g6 == NULL) {
    // invalid pointer g1, g2, g3, g4, g5 or g6
    return false;
  }
  // test between two default games
  bool test1 = game_equal(g1, g2);
  // test between a default game and a default solution game
  bool test2 = game_equal(g1, g3);
  // test between a default game and a new empty game
  bool test3 = game_equal(g1, g4);
  // test between a defaut solution game and a new empty game
  bool test4 = game_equal(g3, g4);
  // test between two default solution games
  bool test5 = game_equal(g3, g5);
  // test between two new empty games
  bool test6 = game_equal(g6, g4);

  // deleting pointers and freeing allocated memory
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);
  game_delete(g5);
  game_delete(g6);

  // creation of two new empty games
  game g7 = game_new_empty();
  game g8 = game_new_empty();
  // value assignment of the square in two game (g7 and g8)
  // with a different value (immutable and not)
  game_set_square(g7, 1, 2, S_ONE);
  game_set_square(g8, 1, 2, S_IMMUTABLE_ONE);
  // test between the games with different values
  bool test7 = game_equal(g7, g8);
  // deleting pointers and freeing allocated memory
  game_delete(g7);
  game_delete(g8);

  if (!test1) {
    // two default games should be equal
    return false;
  }
  if (test2) {
    // a default game and a default solution game should not be equal
    return false;
  }
  if (test3) {
    // a default game and a new empty game should not be equal
    return false;
  }
  if (test4) {
    // a defaut solution game and a new empty game should not be equal
    return false;
  }
  if (!test5) {
    // two default solution games should be equal
    return false;
  }
  if (!test6) {
    // two new empty games should be equal
    return false;
  }
  if (test7) {
    // games with different values
    return false;
  }
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_DELETE  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_delete(void) {
  // creation of a new empty game
  game g = game_new_empty();
  if (g == NULL || sizeof(g) != sizeof(game)) {
    // invalid pointer g
    fprintf(stderr, "Invalid pointer g !\n");
    return false;
  }
  // deleting pointer and freeing allocated memory
  game_delete(g);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_SET_SQUARE  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_set_square(void) {
  // creation of a new empty game
  game g = game_new_empty();
  if (g == NULL || sizeof(g) != sizeof(game)) {
    // invalid pointer g
    return false;
  }
  // value assignment S_ONE of a square
  game_set_square(g, 0, 3, S_ONE);
  if (game_get_square(g, 0, 3) != S_ONE) {
    // incorrect assignment
    game_delete(g);
    return false;
  }
  // value assignment S_ZERO of a square
  game_set_square(g, 0, 3, S_ZERO);
  if (game_get_square(g, 0, 3) != S_ZERO) {
    // incorrect assignment
    game_delete(g);
    return false;
  }
  // value assignment S_EMPTY of a square
  game_set_square(g, 0, 3, S_EMPTY);
  if (game_get_square(g, 0, 3) != S_EMPTY) {
    // incorrect assignment
    game_delete(g);
    return false;
  }
  // value assignment S_IMMUTABLE_ZERO of a square
  game_set_square(g, 0, 3, S_IMMUTABLE_ZERO);
  if (game_get_square(g, 0, 3) != S_IMMUTABLE_ZERO) {
    // incorrect assignment
    game_delete(g);
    return false;
  }
  // value assignment S_IMMUTABLE_ONE of a square
  game_set_square(g, 0, 3, S_IMMUTABLE_ONE);
  if (game_get_square(g, 0, 3) != S_IMMUTABLE_ONE) {
    // incorrect assignment
    game_delete(g);
    return false;
  }
  // deleting pointers and freeing allocated memory
  game_delete(g);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_GET_SQUARE  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_get_square() {
  // pointer tab (toward a square structure) allocation to create a new array
  square *tab =
      (square *)malloc((DEFAULT_SIZE * DEFAULT_SIZE) * sizeof(square));
  for (int k = 0; k < DEFAULT_SIZE * DEFAULT_SIZE; k++) {
    // initializing each square to an empty square
    tab[k] = S_EMPTY;
  }
  // value assignment of some squares
  tab[0] = S_ONE;
  tab[1] = S_ZERO;
  tab[2] = S_IMMUTABLE_ZERO;
  tab[3] = S_IMMUTABLE_ONE;
  // creation of a new game
  game g = game_new(tab);
  if (g == NULL || sizeof(g) != sizeof(game)) {
    // invalid pointer g
    return false;
  }
  if (game_get_square(g, 0, 0) != S_ONE) {
    // square (0,0) should have S_ONE as value
    // deleting pointers and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 0, 1) != S_ZERO) {
    // square (0,1) should have S_ZERO as value
    // deleting pointers and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 0, 2) != S_IMMUTABLE_ZERO) {
    // square (0,2) should have S_IMMUTABLE_ZERO as value
    // deleting pointers and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 0, 3) != S_IMMUTABLE_ONE) {
    // square (0,3) should have S_IMMUTABLE_ONE as value
    // deleting pointers and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 0, 4) != S_EMPTY) {
    // square (0,4) should have S_EMPTY as value
    // deleting pointers and freeing allocated memory
    game_delete(g);
    return false;
  }
  // deleting pointers and freeing allocated memory
  free(tab);
  game_delete(g);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_GET_NUMBER  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_get_number(void) {
  // pointer tab (toward a square structure) allocation to create a new array
  square *tab =
      (square *)malloc((DEFAULT_SIZE * DEFAULT_SIZE) * sizeof(square));
  for (int i = 0; i < (DEFAULT_SIZE * DEFAULT_SIZE); i++) {
    // initializing each square to an empty square
    tab[i] = S_EMPTY;
  }
  // value assignment of some squares
  tab[0] = S_ONE;
  tab[1] = S_ZERO;
  tab[2] = S_IMMUTABLE_ONE;
  tab[3] = S_IMMUTABLE_ZERO;
  // creation of a new game
  game g = game_new(tab);
  if ((g == NULL) || (sizeof(g) != sizeof(game))) {
    // invalid pointer g : freeing allocated memory
    free(tab);
    return false;
  }
  if (game_get_number(g, 0, 0) != 1) {
    // square (0,0) should have 1 as value
    // deleting pointers and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_number(g, 0, 1) != 0) {
    // square (0,1) should have 0 as value
    // deleting pointers and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_number(g, 0, 2) != 1) {
    // square (0,2) should have 1 as value
    // deleting pointers and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_number(g, 0, 3) != 0) {
    // square (0,3) should have 0 as value
    // deleting pointers and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_number(g, 0, 4) != -1) {
    // square (0,4) should have -1 as value
    // deleting pointers and freeing allocated memory
    game_delete(g);
    return false;
  }
  // deleting pointers and freeing allocated memory
  game_delete(g);
  free(tab);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_GET_NEXT_SQUARE  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_get_next_square() {
  // creation of a default solution game
  game g = game_default_solution();
  if (g == NULL || sizeof(g) != sizeof(game)) {
    // invalid pointer g
    return false;
  }
  for (int i = 0; i < (DEFAULT_SIZE); i++) {
    for (int j = 0; j < (DEFAULT_SIZE); j++) {
      for (int d = 0; d < 3; d++) {
        if (i - d < 0) {
          // top overflow (without wrapping option)
          if (game_get_next_square(g, i, j, UP, d) != -1) {
            // finding the value of a square by moving vertically upwards
            // incorrect return value, it should be -1 because of the overflow
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        } else {
          if ((int)game_get_next_square(g, i, j, UP, d) !=
              (int)game_get_square(g, i - d, j)) {
            // finding the value of a square by moving vertically upwards
            // incorrect return value, it is not the square's value
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        }
        if (i + d > DEFAULT_SIZE - 1) {
          // bottom overflow (without wrapping option)
          if (game_get_next_square(g, i, j, DOWN, d) != -1) {
            // finding the value of a square by moving vertically downwards
            // incorrect return value, it should be -1 because of the overflow
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        } else {
          if ((int)game_get_next_square(g, i, j, DOWN, d) !=
              (int)game_get_square(g, i + d, j)) {
            // finding the value of a square by moving vertically downwards
            // incorrect return value, it is not the square's value
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        }
        if (j - d < 0) {
          // left overflow (without wrapping option)
          if (game_get_next_square(g, i, j, LEFT, d) != -1) {
            // finding the value of a square by moving horizontally to the left
            // incorrect return value, it should be -1 because of the overflow
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        } else {
          // we try to find the value of the left square
          if ((int)game_get_next_square(g, i, j, LEFT, d) !=
              (int)game_get_square(g, i, j - d)) {
            // finding the value of a square by moving horizontally to the left
            // incorrect return value, it is not the square's value
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        }
        if (j + d > DEFAULT_SIZE - 1) {
          // right overflow (without wrapping option)
          if (game_get_next_square(g, i, j, RIGHT, d) != -1) {
            // finding the value of a square by moving horizontally to the right
            // incorrect return value, it should be -1 because of the overflow
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        } else {
          // we try to find the value of the right square
          if ((int)game_get_next_square(g, i, j, RIGHT, d) !=
              (int)game_get_square(g, i, j + d)) {
            // finding the value of a square by moving horizontally to the right
            // incorrect return value, it is not the square's value
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        }
      }
    }
  }
  // deleting pointer and freeing allocated memory
  game_delete(g);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_GET_NEXT_NUMBER  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_get_next_number() {
  // creation of a default solution game
  game g = game_default_solution();
  if (g == NULL) {
    // invalid pointer g
    return false;
  }
  for (int i = 0; i < (DEFAULT_SIZE); i++) {
    for (int j = 0; j < (DEFAULT_SIZE); j++) {
      for (int d = 0; d < 3; d++) {
        if (i - d < 0) {
          // top overflow (without wrapping option)
          if (game_get_next_number(g, i, j, UP, d) != -1) {
            // finding the value of a square by moving vertically upwards
            // incorrect return value, it should be -1 because of the overflow
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        } else {
          if (game_get_next_number(g, i, j, UP, d) !=
              game_get_number(g, i - d, j)) {
            // finding the value of a square by moving vertically upwards
            // incorrect return value, it is not the square's value
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        }
        if (i + d > 5) {
          // down overflow (without wrapping option)
          if (game_get_next_number(g, i, j, DOWN, d) != -1) {
            // finding the value of a square by moving vertically downwards
            // incorrect return value, it should be -1 because of the overflow
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        } else {
          if (game_get_next_number(g, i, j, DOWN, d) !=
              game_get_number(g, i + d, j)) {
            // finding the value of a square by moving vertically downwards
            // incorrect return value, it is not the square's value
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        }
        if (j - d < 0) {
          // left overflow (without wrapping option)
          if (game_get_next_number(g, i, j, LEFT, d) != -1) {
            // finding the value of a square by moving hotizontally to the left
            // incorrect return value, it should be -1 because of the overflow
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        } else {
          if (game_get_next_number(g, i, j, LEFT, d) !=
              game_get_number(g, i, j - d)) {
            // finding the value of a square by moving hotizontally to the left
            // incorrect return value, it is not the square's value
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        }
        if (j + d > 5) {
          // right overflow (without wrapping option)
          if (game_get_next_number(g, i, j, RIGHT, d) != -1) {
            // finding the value of a square by moving hotizontally to the right
            // incorrect return value, it should be -1 because of the overflow
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        } else {
          if (game_get_next_number(g, i, j, RIGHT, d) !=
              game_get_number(g, i, j + d)) {
            // finding the value of a square by moving hotizontally to the right
            // incorrect return value, it is not the square's value
            // deleting pointer and freeing allocated memory
            game_delete(g);
            return false;
          }
        }
      }
    }
  }
  // deleting pointer and freeing allocated memory
  game_delete(g);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_IS_EMPTY  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_is_empty(void) {
  // creation of a default game
  game g = game_default();
  // value assigment of some squares
  game_set_square(g, 0, 3, S_ONE);
  game_set_square(g, 0, 4, S_ZERO);
  bool test1 = game_is_empty(g, 1, 2);  // S_EMPTY
  bool test2 = !game_is_empty(
      g, 0, 1);  // S_IMMUTABLE_ONE (definition of a default game)
  bool test3 = !game_is_empty(
      g, 0, 2);  // S_IMMUTABLE_ZERO (definition of a default game)
  bool test4 = !game_is_empty(g, 0, 3);  // S_ONE
  bool test5 = !game_is_empty(g, 0, 4);  // S_ZERO
  // deleting pointer and freeing allocated memory
  game_delete(g);
  if (test1 && test2 && test3 && test4 && test5) {
    // passed test !
    return true;
  }
  // test failed...
  return false;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_IS_IMMUTABLE  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_is_immutable(void) {
  // creation of a default game
  game g = game_default();
  if (g == NULL || sizeof(g) != sizeof(game)) {
    // invalid pointer g
    return false;
  }
  // following squares should be immutable squares
  if (game_get_square(g, 0, 1) != S_IMMUTABLE_ONE) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 0, 2) != S_IMMUTABLE_ZERO) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 2, 1) != S_IMMUTABLE_ZERO) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 2, 4) != S_IMMUTABLE_ZERO) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 3, 1) != S_IMMUTABLE_ZERO) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 3, 2) != S_IMMUTABLE_ONE) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 4, 2) != S_IMMUTABLE_ONE) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 4, 5) != S_IMMUTABLE_ZERO) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_get_square(g, 5, 5) != S_IMMUTABLE_ZERO) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  // deleting pointer and freeing allocated memory
  game_delete(g);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_HAS_ERROR  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_has_error() {
  // creation of a default solution game
  game g = game_default_solution();
  if (g == NULL) {
    // invalid pointer g
    return false;
  }
  for (int i = 0; i < (DEFAULT_SIZE); i++) {
    for (int j = 0; j < (DEFAULT_SIZE); j++) {
      if (game_has_error(g, i, j) != 0) {
        // game should not have any error because it is a default solution game
        // deleting pointer and freeing allocated memory
        game_delete(g);
        return false;
      }
    }
  }
  // deleting pointer and freeing allocated memory
  game_delete(g);
  // creation of a new empty game
  game g1 = game_new_empty();
  if (g1 == NULL) {
    // invalid pointer g1
    return false;
  }
  for (int i = 0; i < 3; i++) {
    // value assigment of some squares
    game_set_square(g1, 0, i, S_ZERO);
    game_set_square(g1, i, 0, S_ZERO);
  }
  for (int i = 0; i < 3; i++) {
    if (game_has_error(g1, 0, i) == 0) {
      // the game should have an error because of three followed equal squares
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
    if (game_has_error(g1, i, 0) == 0) {
      // the game should have an error because of three followed equal squares
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
  }
  for (int i = 0; i < 3; i++) {
    // value assigment of some squares
    game_set_square(g1, 0, i, S_ONE);
    game_set_square(g1, i, 0, S_ONE);
  }
  for (int i = 0; i < 3; i++) {
    if (game_has_error(g1, 0, i) == 0) {
      // the game should have an error because of three followed equal squares
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
    if (game_has_error(g1, i, 0) == 0) {
      // the game should have an error because of three followed equal squares
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
  }
  // deleting pointer and freeing allocated memory
  game_delete(g1);
  // creation of a new empty game
  game g2 = game_new_empty();
  // value assigment of some squares
  for (int i = 0; i < 2; i++) {
    game_set_square(g2, 0, i, S_ZERO);
    game_set_square(g2, i, 0, S_ZERO);
    game_set_square(g2, 0, 5 - i, S_ZERO);
    game_set_square(g2, 5 - i, 0, S_ZERO);
  }
  for (int i = 0; i < 2; i++) {
    if (game_has_error(g2, 0, i) == 0) {
      // the game should have an error because of three followed equal squares
      // looking at both ends of each line
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
    if (game_has_error(g2, 0, 5 - i) == 0) {
      // the game should have an error because of three followed equal squares
      // looking at both ends of each line
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
    if (game_has_error(g2, i, 0) == 0) {
      // the game should have an error because of three followed equal squares
      // looking at both ends of each column
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
    if (game_has_error(g2, 5 - i, 0) == 0) {
      // the game should have an error because of three followed equal squares
      // looking at both ends of each column
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
  }
  // value assigment of some squares
  for (int i = 0; i < 2; i++) {
    game_set_square(g2, 0, i, S_ONE);
    game_set_square(g2, i, 0, S_ONE);
    game_set_square(g2, 0, 5 - i, S_ONE);
    game_set_square(g2, 5 - i, 0, S_ONE);
  }
  for (int i = 0; i < 2; i++) {
    if (game_has_error(g2, 0, i) == 0) {
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
    if (game_has_error(g2, 0, 5 - i) == 0) {
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
    if (game_has_error(g2, i, 0) == 0) {
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
    if (game_has_error(g2, 5 - i, 0) == 0) {
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
  }
  game_delete(g2);
  // creation of a new empty game with options to test with the wrapping option
  game g3 = game_new_empty_ext(6, 8, true, false);
  game_play_move(g3, 0, 0, S_ONE);
  game_play_move(g3, 0, 1, S_ONE);
  game_play_move(g3, 0, 7, S_ONE);
  if (game_has_error(g3, 0, 0) == 0) {
    // the game should have an error because of three followed equal squares
    game_delete(g3);
    return false;
  }
  game_delete(g3);
  // creation of a new empty game with options to test with the unique option
  game g4 = game_new_empty_ext(6, 6, false, true);
  game_play_move(g4, 0, 0, S_ONE);
  game_play_move(g4, 0, 1, S_ZERO);
  game_play_move(g4, 0, 2, S_ONE);
  game_play_move(g4, 0, 3, S_ZERO);
  game_play_move(g4, 0, 4, S_ONE);
  game_play_move(g4, 0, 5, S_ZERO);

  game_play_move(g4, 1, 0, S_ONE);
  game_play_move(g4, 1, 1, S_ZERO);
  game_play_move(g4, 1, 2, S_ONE);
  game_play_move(g4, 1, 3, S_ZERO);
  game_play_move(g4, 1, 4, S_ONE);
  game_play_move(g4, 1, 5, S_ZERO);
  if (game_has_error(g4, 0, 0) == 0) {
    // the game should have an error because of three followed equal squares
    // deleting pointer and freeing allocated memory
    game_delete(g4);
    return false;
  }
  // deleting pointer and freeing allocated memory
  game_delete(g4);
  // passed test !
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_CHECK_MOVE  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_check_move(void) {
  // creation of a new empty game
  game g = game_new_empty();
  if (g == NULL || sizeof(g) != sizeof(game)) {
    // invalid pointer g
    return false;
  }
  game_set_square(g, 0, 0, S_ONE);
  game_set_square(g, 0, 1, S_ZERO);
  game_set_square(g, 0, 2, S_IMMUTABLE_ZERO);
  game_set_square(g, 0, 3, S_IMMUTABLE_ONE);
  if (!game_check_move(g, 0, 0, S_ONE)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (!game_check_move(g, 0, 0, S_ZERO)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (!game_check_move(g, 0, 0, S_EMPTY)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (!game_check_move(g, 0, 1, S_ONE)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (!game_check_move(g, 0, 1, S_ZERO)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (!game_check_move(g, 0, 1, S_EMPTY)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 0, 2, S_ONE)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 0, 2, S_ZERO)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 0, 2, S_EMPTY)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 0, 3, S_ZERO)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 0, 3, S_ONE)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (game_check_move(g, 0, 3, S_EMPTY)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (!game_check_move(g, 0, 4, S_EMPTY)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (!game_check_move(g, 0, 4, S_ONE)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  if (!game_check_move(g, 0, 4, S_ZERO)) {
    // deleting pointer and freeing allocated memory
    game_delete(g);
    return false;
  }
  for (unsigned int i = DEFAULT_SIZE; i < 50; i++) {
    if (game_check_move(g, 1, i, S_ZERO)) {
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
    if (game_check_move(g, i, 1, S_EMPTY)) {
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
    if (game_check_move(g, i, i, S_ONE)) {
      // deleting pointer and freeing allocated memory
      game_delete(g);
      return false;
    }
  }
  // deleting pointer and freeing allocated memory
  game_delete(g);
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_PLAY_MOVE  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_play_move() {
  // creation of a default solution game
  game g = game_default_solution();
  if (g == NULL || sizeof(g) != sizeof(game)) {
    // invalid pointer g
    return false;
  }
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != S_IMMUTABLE_ONE &&
          game_get_square(g, i, j) != S_IMMUTABLE_ZERO) {
        // the square (i,j) can be chosen to play a move
        // value assignment S_EMPTY by playing a move
        game_play_move(g, i, j, S_EMPTY);
        if (game_get_square(g, i, j) != S_EMPTY) {
          // error : incorrect assignment, the move was not made correctly
          game_delete(g);
          return false;
        }
        // value assignment S_ZERO by playing a move
        game_play_move(g, i, j, S_ZERO);
        if (game_get_square(g, i, j) != S_ZERO) {
          // error : incorrect assignment, the move was not made correctly
          game_delete(g);
          return false;
        }
        // value assignment S_ONE by playing a move
        game_play_move(g, i, j, S_ONE);
        if (game_get_square(g, i, j) != S_ONE) {
          // error : incorrect assignment, the move was not made correctly
          game_delete(g);
          return false;
        }
      }
    }
  }
  // passed test !
  game_delete(g);
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_IS_OVER  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_is_over() {
  game g = game_default_solution();
  if (g == NULL) {
    return false;
  }
  if (!game_is_over(g)) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  game g1 = game_new_empty();
  if (g1 == NULL) {
    return false;
  }
  if (game_is_over(g1)) {
    game_delete(g1);
    return false;
  }
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < 3; j++) {
      if (i % 2 == 0) {
        game_set_square(g1, i, j, S_ZERO);
        game_set_square(g1, i, 5 - j, S_ONE);
      } else {
        game_set_square(g1, i, j, S_ONE);
        game_set_square(g1, i, 5 - j, S_ZERO);
      }
    }
  }
  if (game_is_over(g1)) {
    game_delete(g1);
    return false;
  }
  game g2 = game_new_empty_ext(4, 4, false, true);
  game_set_square(g2, 0, 0, S_ONE);
  game_set_square(g2, 0, 1, S_ZERO);
  game_set_square(g2, 0, 2, S_ONE);
  game_set_square(g2, 0, 3, S_ZERO);

  game_set_square(g2, 1, 0, S_ONE);
  game_set_square(g2, 1, 1, S_ZERO);
  game_set_square(g2, 1, 2, S_ONE);
  game_set_square(g2, 1, 3, S_ZERO);

  game_set_square(g2, 2, 0, S_ZERO);
  game_set_square(g2, 2, 1, S_ONE);
  game_set_square(g2, 2, 2, S_ZERO);
  game_set_square(g2, 2, 3, S_ONE);

  game_set_square(g2, 3, 0, S_ZERO);
  game_set_square(g2, 3, 1, S_ONE);
  game_set_square(g2, 3, 2, S_ZERO);
  game_set_square(g2, 3, 3, S_ONE);

  if (game_is_over(g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g2);
  game_delete(g1);
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_RESTART  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_restart() {
  game g1 = game_default();
  game g2 = game_default_solution();

  if (g1 == NULL && g2 == NULL) {
    return false;
  }
  game_play_move(g2, 0, 0, S_ONE);
  game_play_move(g2, 1, 0, S_ONE);
  game_undo(g2);
  game_restart(g2);
  game_redo(g2);
  if (game_get_square(g2, 1, 0) == S_ONE) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  bool a = game_equal(g1, g2);
  game_delete(g1);
  game_delete(g2);
  return a;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_PRINT  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_print(void) {
  game g = game_default();
  if ((g == NULL) || (sizeof(g) != sizeof(game))) {
    return false;
  }
  game_print(g);
  game_delete(g);
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_DEFAULT  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_default(void) {  // On utilise game_default,  un MALLOC,  un
                                // FREE,  game_delete,  game_get_square
  game g = game_default();
  if (g == NULL || sizeof(g) != sizeof(game)) {
    return false;
  }
  square *tab =
      (square *)malloc((DEFAULT_SIZE * DEFAULT_SIZE) * sizeof(square));
  for (int k = 0; k < DEFAULT_SIZE * DEFAULT_SIZE; k++) {
    tab[k] = S_EMPTY;
  }
  tab[1] = S_IMMUTABLE_ONE;
  tab[2] = S_IMMUTABLE_ZERO;
  tab[13] = S_IMMUTABLE_ZERO;
  tab[16] = S_IMMUTABLE_ZERO;
  tab[19] = S_IMMUTABLE_ZERO;
  tab[20] = S_IMMUTABLE_ONE;
  tab[26] = S_IMMUTABLE_ONE;
  tab[29] = S_IMMUTABLE_ZERO;
  tab[35] = S_IMMUTABLE_ZERO;
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (tab[i * DEFAULT_SIZE + j] != game_get_square(g, i, j)) {
        game_delete(g);
        return false;
      }
    }
  }
  free(tab);
  game_delete(g);
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_DEFAULT_SOLUTION  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_default_solution(void) {
  // creation of a default game
  game g = game_default_solution();
  if (g == NULL || sizeof(g) != sizeof(game)) {
    // invalid pointer g
    return false;
  }
  if (game_is_over(g)) {
    for (uint i = 0; i < DEFAULT_SIZE; i++) {
      for (uint j = 0; j < DEFAULT_SIZE; j++) {
        if (game_has_error(g, i, j) != 0) {
          // error : this square shouldn't have an error
          game_delete(g);
          return false;
        }
      }
    }
    // passed test !
    game_delete(g);
    return true;
  }
  // error : the game is not over, it can't be a default solution
  game_delete(g);
  return false;
}

/*
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
  * * * * *  V2 TESTS  * * * * *
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
*/

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_NEW_EXT  * * * * *
---------------------------------------------------------------------------------------------
*/
bool test_game_new_ext(uint nb_rows, uint nb_cols) {
  square tab[nb_cols * nb_rows];
  for (uint k = 0; k < nb_cols * nb_rows; k++) {
    tab[k] = S_EMPTY;
  }
  game g = game_new_ext(nb_rows, nb_cols, tab, false, false);
  if (g == NULL) {
    return false;
  }
  if (game_nb_cols(g) != nb_cols || game_nb_rows(g) != nb_rows) {
    game_delete(g);
    return false;
  }
  if (game_is_unique(g) != false || game_is_wrapping(g) != false) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_NEW_EMPTY_EXT  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_new_empty_ext(uint nb_rows, uint nb_cols) {
  game g = game_new_empty_ext(nb_rows, nb_cols, false, false);
  if ((g == NULL)) {
    fprintf(stderr, "TEST ERROR : Invalid pointer g !\n");
    return false;
  }
  if (game_nb_rows(g) != nb_rows || game_nb_cols(g) != nb_cols) {
    game_delete(g);
    return false;
  }
  if (game_is_unique(g) != false || game_is_wrapping(g) != false) {
    game_delete(g);
    return false;
  }
  for (unsigned int i = 0; i < g->size_line; i++) {
    for (unsigned int j = 0; j < g->size_column; j++) {
      if (game_get_square(g, i, j) != S_EMPTY) {
        fprintf(stderr, "TEST ERROR : a non-empty square exists !\n");
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_UNDO  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_undo() {
  game g = game_new_empty_ext(6, 8, false, false);
  game_play_move(g, 0, 0, S_ONE);
  game_play_move(g, 0, 1, S_ZERO);
  game_undo(g);
  if (game_get_square(g, 0, 0) != S_ONE ||
      game_get_square(g, 0, 1) != S_EMPTY) {
    game_delete(g);
    return false;
  }
  game_play_move(g, 0, 5, S_ONE);
  game_undo(g);
  if (game_get_square(g, 0, 5) == S_ONE) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_REDO  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_redo() {
  game g = game_new_empty_ext(6, 8, false, false);
  game_play_move(g, 0, 0, S_ONE);
  game_undo(g);
  game_redo(g);
  if (game_get_square(g, 0, 0) != S_ONE) {
    game_delete(g);
    return false;
  }
  game_undo(g);
  // game_play_move(g, 2, 3, S_ONE);
  // game_play_move(g, 2, 3, S_ZERO);
  // game_undo(g);
  // game_redo(g);
  // game_undo(g);
  if (game_get_square(g, 0, 0) == S_ONE) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}

/*game_play_move(g, 0, 5, S_ONE);
game_play_move(g, 4, 3, S_ZERO);
game_undo(g);
game_undo(g);
game_redo(g);
if (game_get_square(g, 0, 5) != S_ONE) {
  game_delete(g);
  return false;
}
if (game_get_square(g, 4, 3) == S_ZERO) {
  game_delete(g);
  return false;
}
game_delete(g);
return true;
}*/

/*
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
  * * * * *  GAME_IO TESTS  * * * * *
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
*/
/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_LOAD  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_load() {
  game g = game_default_solution();
  game g1 = game_load("solution.txt");

  for (unsigned int i = 0; i < game_nb_rows(g); i++) {
    for (unsigned int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) != game_get_square(g1, i, j)) {
        game_delete(g);
        game_delete(g1);
        return false;
      }
    }
  }
  game_delete(g);
  game_delete(g1);

  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_SAVE  * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_save() {
  game g1 = game_default();
  game_save(g1, "test1.txt");
  game g2 = game_load("test1.txt");
  if (!game_equal(g1, g2)) {
    remove("test1.txt");
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_delete(g1);
  game_delete(g2);
  remove("test1.txt");

  game g3 = game_default_solution();
  game_save(g3, "test2.txt");
  game g4 = game_load("test2.txt");
  if (!game_equal(g3, g4)) {
    game_delete(g3);
    game_delete(g4);
    remove("test2.txt");
    return false;
  }
  game_delete(g3);
  game_delete(g4);
  remove("test2.txt");
  return true;
}

/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_GAME_NB_SOLUTIONS* * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_nb_solutions() {
  game g = game_default();
  if (game_nb_solutions(g) < 1) {
    game_delete(g);
    return false;
  }
  game_delete(g);
  return true;
}
/*
---------------------------------------------------------------------------------------------
  * * * * *  TEST_GAME_SOLVE * * * * *
---------------------------------------------------------------------------------------------
*/

bool test_game_solve() {
  return true;
}

/*
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
  * * * * *  MAIN ROUTINE  * * * * *
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------
*/

int main(int argc, char *argv[]) {
  bool ok = false;
  if (argc != 2) {
    exit(EXIT_FAILURE);
  }

  if (strcmp("game_is_immutable", argv[1]) == 0) {
    ok = test_game_is_immutable();
  } else if (strcmp("game_default_solution", argv[1]) == 0) {
    ok = test_game_default_solution();
  } else if (strcmp("game_set_square", argv[1]) == 0) {
    ok = test_game_set_square();
  } else if (strcmp("game_copy", argv[1]) == 0) {
    ok = test_game_copy();
  } else if (strcmp("game_play_move", argv[1]) == 0) {
    ok = test_game_play_move();
  } else if (strcmp("game_get_next_square", argv[1]) == 0) {
    ok = test_game_get_next_square();
  } else if (strcmp("game_undo", argv[1]) == 0) {
    ok = test_game_undo();
  } else if (strcmp("game_new_ext", argv[1]) == 0) {
    ok = test_game_new_ext(10, 8);

  } else if (strcmp("game_is_over", argv[1]) == 0) {
    ok = test_game_is_over();
  } else if (strcmp("game_default", argv[1]) == 0) {
    ok = test_game_default();
  } else if (strcmp("game_equal", argv[1]) == 0) {
    ok = test_game_equal();
  } else if (strcmp("game_new", argv[1]) == 0) {
    ok = test_game_new();
  } else if (strcmp("game_get_square", argv[1]) == 0) {
    ok = test_game_get_square();
  } else if (strcmp("game_get_next_number", argv[1]) == 0) {
    ok = test_game_get_next_number();
  } else if (strcmp("game_has_error", argv[1]) == 0) {
    ok = test_game_has_error();
  } else if (strcmp("game_redo", argv[1]) == 0) {
    ok = test_game_redo();
  } else if (strcmp("game_new_empty_ext", argv[1]) == 0) {
    ok = test_game_new_empty_ext(10, 8);

  } else if (strcmp("game_new_empty", argv[1]) == 0) {
    ok = test_game_new_empty();
  } else if (strcmp("game_delete", argv[1]) == 0) {
    ok = test_game_delete();
  } else if (strcmp("game_get_number", argv[1]) == 0) {
    ok = test_game_get_number();
  } else if (strcmp("game_is_empty", argv[1]) == 0) {
    ok = test_game_is_empty();
  } else if (strcmp("game_restart", argv[1]) == 0) {
    ok = test_game_restart();
  } else if (strcmp("game_print", argv[1]) == 0) {
    ok = test_game_print();
  } else if (strcmp("game_check_move", argv[1]) == 0) {
    ok = test_game_check_move();
  } else if (strcmp("game_load", argv[1]) == 0) {
    ok = test_game_load();
  } else if (strcmp("game_save", argv[1]) == 0) {
    ok = test_game_save();
  } else if (strcmp("game_solve",argv[1]) == 0) {
    ok = test_game_solve();
  } else if (strcmp("game_nb_solutions",argv[1]) == 0) {
    ok = test_game_nb_solutions();

  } else {
    exit(EXIT_FAILURE);
  }
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
