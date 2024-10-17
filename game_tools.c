#include "game_tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

//#include "game.h"
#include "game_ext.h"
#include "game.h"
#include "game_aux.h"
#define NB_ROWS 4
#define NB_COLS 4

square char_to_square(char x) {
  if (x == 'e') {
    return S_EMPTY;
  }
  if (x == 'w') {
    return S_ZERO;
  }
  if (x == 'b') {
    return S_ONE;
  }
  if (x == 'W') {
    return S_IMMUTABLE_ZERO;
  }
  if (x == 'B') {
    return S_IMMUTABLE_ONE;
  } else {
    exit(EXIT_FAILURE);
  }
}
/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_LOAD  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Creates a game by loading its description from a text file.
  RETURN : Returns the loaded game.
---------------------------------------------------------------------------------------------
*/
game game_load(char* filename) {
  if (filename == NULL) {
    fprintf(stderr, "Invalid pointer filename !");
    exit(EXIT_FAILURE);
  }
  // ouverture d'un fichier en lecture seule
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Invalid pointer file !\n");
    exit(EXIT_FAILURE);
  }
 /* déclaration des variables du jeu intervenant avec les éléments lu dans le fichier*/

  uint nb_rows;
  uint nb_cols;
  uint wrapping;
  uint unique;

  fscanf(file ,"%d %d %d %d", &nb_rows, &nb_cols, &wrapping, &unique);
    square *s = malloc (nb_rows * nb_cols * sizeof(square));
    if (s == NULL) {
      fprintf(stderr, "Invalid pointer s! \n");
      exit(EXIT_FAILURE);
    }
  int position = 0;
  
  while (!feof(file)){
    char y = fgetc(file);
      if(y=='W'){
        s[position]= S_IMMUTABLE_ZERO;
        position+=1;
      }else if (y=='B'){
        s[position]= S_IMMUTABLE_ONE;
        position+=1;
      }else if (y=='w'){
        s[position]= S_ZERO;
        position+=1;
      }else if (y=='b'){
        s[position]= S_ONE;
        position+=1;
      }else if (y=='e'){
        s[position]= S_EMPTY;
        position+=1;
      }
    
  }  
    // création du jeu à partir de chaque paramètre créé
    game g = game_new_ext (nb_rows, nb_cols, s, wrapping, unique);
    free(s);
    fclose(file);
    return g;
  
}  
/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_SAVE  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Saves a game in a text file.
---------------------------------------------------------------------------------------------
*/
void game_save(cgame g, char* filename) {
  if (g == NULL) {
    fprintf(stderr, "Invalid pointer g !");
    exit(EXIT_FAILURE);
  }
  if (filename == NULL) {
    fprintf(stderr, "Invalid pointer filename !");
    exit(EXIT_FAILURE);
  }
  // ouverture d'un fichier en lecture seule
  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "Invalid pointer file !\n");
    exit(EXIT_FAILURE);
  }
  // création de variables contenant les paramètres du jeu
  uint nb_rows = game_nb_rows(g);
  uint nb_cols = game_nb_cols(g);
  uint wrapping = game_is_wrapping(g);
  uint unique = game_is_unique(g);
  // affichage de ces paramètres
  fprintf(file, "%u %u %u %u\n", nb_rows, nb_cols, wrapping, unique);
  for (uint i = 0; i < game_nb_rows(g); i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
      square s = game_get_square(g, i, j);
      if (s == S_EMPTY) {
        // empty square
        fprintf(file, "e");
      } else if (s == S_IMMUTABLE_ZERO) {
        // immutable zero square
        fprintf(file, "W");
      } else if (s == S_IMMUTABLE_ONE) {
        // immutable zero one
        fprintf(file, "B");
      } else if (s == S_ZERO) {
        fprintf(file, "w");
        // zero square
      } else if (s == S_ONE) {
        // one square
        fprintf(file, "b");
      }
    }
    // affichage d'un retour à la ligne
    fprintf(file, "\n");
  }
  // fermeture du fichier
  fclose(file);
}
/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_SOLVE  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Computes the solution of a given game.
  RETURN : Returns true if a solution is found, false otherwise.
---------------------------------------------------------------------------------------------
*/
void printGame(game g) {
  for (uint i = 0; i < game_nb_rows(g)-1; i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
      printf("%d", game_get_number(g,i,j));
    }
    printf("|");
  }
  for (uint i = 0; i < game_nb_cols(g); i++){
    printf("%d", game_get_number(g,game_nb_rows(g)-1,i));

  }
  printf("\n");
}

void genGame(game g, uint pos, int *tab, uint *count, bool stop_first) {
  // one solution found
  if (stop_first && *count > 0) {
    printGame(g);
    return;
  }
  if (game_is_over(g)) {
    (*count)++;
    printGame(g);
    if (stop_first) {
      // one solution found
      return;
    }
  }
  // stop recursive calls
  if (pos == game_nb_cols(g) * game_nb_rows(g)) {
    return;
  }
  uint j = pos % game_nb_cols(g);
  uint i = pos / game_nb_cols(g);
  // immutable square : next square !
  if (game_is_immutable(g, i, j)){
    genGame(g, pos+1, tab, count, stop_first);
  }
  else {
    // maybe a S_ZERO square ?
    //if (stop_first && *count < 1) {
      game_set_square(g, i, j, S_ZERO);
      tab[pos]=0;
      genGame(g, pos+1, tab, count, stop_first);
    
    // maybe a S_ONE square ?
    //if (stop_first && *count < 1) {
      game_set_square(g, i, j, S_ONE);
      tab[pos]=1;
      genGame(g, pos+1, tab, count, stop_first);
    }


}


bool game_solve(game g) {
  assert(g != NULL);
  game g_aux = game_copy(g);
  game_restart(g_aux);
  uint size = game_nb_cols(g) * game_nb_rows(g);
  int tab[size];
  uint count = 0;
  genGame(g_aux, 0,tab, &count, true);
  if (count == 0){
    free(g_aux);
    return false;
  }
  free(g_aux);
  return true;
}


/*
---------------------------------------------------------------------------------------------
  * * * * *  GAME_NB_SOLUTIONS  * * * * *
---------------------------------------------------------------------------------------------
  DESCRIPTION : Computes the total number of solutions of a given game.
  RETURN : Returns the number of solutions.
---------------------------------------------------------------------------------------------
*/

uint game_nb_solutions(cgame g) {
  assert(g != NULL);
  game g_aux = game_copy(g);
  game_restart(g_aux);
  uint size = game_nb_cols(g) * game_nb_rows(g);
  int tab[size];
  uint count = 0;
  genGame(g_aux, 0, tab, &count, false);  // avec true on obtient 10% au lieu de 5% mais ça doit être faux
  free(g_aux);
  return count;
}

////////////////////////////////

/* ************************************************************************** */
/*                            Random Game Generator                           */
/* ************************************************************************** */

static game game_set_some_mutable_squares(game g, uint nb_empty)
{
  assert(g);
  uint nb_rows = game_nb_rows(g);
  uint nb_cols = game_nb_cols(g);
  uint nb_squares = nb_rows * nb_cols;
  assert(nb_empty <= nb_squares);
  uint nb_empty1 = nb_empty;
  while (nb_empty1 > 0) {
    uint pos = rand() % nb_squares;
    uint row = pos / nb_cols;
    uint col = pos % nb_cols;
    if (game_is_immutable(g, row, col)) {
      square s = game_get_number(g, row, col);
      game_set_square(g, row, col, s + 1);
      nb_empty1--;
    }
  }
  return g;
}

/* ************************************************************************** */

static void game_flip_one_square(game g, int i, int j)
{
  square s = game_get_square(g, i, j);
  if (s == S_IMMUTABLE_ONE)
    game_set_square(g, i, j, S_IMMUTABLE_ZERO);
  else
    game_set_square(g, i, j, S_IMMUTABLE_ONE);
}

/* ************************************************************************** */

static bool try_flip(game g, int i1, int j1, int i2, int j2)
{
  if (i1 == i2 && j1 == j2) return false;
  assert(game_is_over(g));
  square s11 = game_get_square(g, i1, j1);
  square s22 = game_get_square(g, i2, j2);
  square s12 = game_get_square(g, i1, j2);
  square s21 = game_get_square(g, i2, j1);

  if (s11 == s22 && s12 == s21 && s11 != s12) {
    game_flip_one_square(g, i1, j1);
    game_flip_one_square(g, i2, j2);
    game_flip_one_square(g, i1, j2);
    game_flip_one_square(g, i2, j1);

    if (game_is_over(g)) {
      return true;
    } else {  // undo
      game_flip_one_square(g, i1, j1);
      game_flip_one_square(g, i2, j2);
      game_flip_one_square(g, i1, j2);
      game_flip_one_square(g, i2, j1);
      assert(game_is_over(g));
      return false;
    }
  }
  return false;
}

/* ************************************************************************** */

static void apply_nb_flips(game g, uint nb)
{
  uint nb_rows = game_nb_rows(g);
  uint nb_cols = game_nb_cols(g);
  while (nb > 0) {
    uint i1 = rand() % nb_rows;
    uint j1 = rand() % nb_cols;
    uint i2 = rand() % nb_rows;
    uint j2 = rand() % nb_cols;
    if (try_flip(g, i1, j1, i2, j2)) nb--;
  }
}

/* ************************************************************************** */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint uniq, bool with_solution)
{
  // step 0: create an empty game
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping, uniq);

  // fill the grid with initial values
  for (uint i = 0; i < nb_rows; i++)
    for (uint j = 0; j < nb_cols; j++) game_set_square(g, i, j, (i + j) % 2 + 3);

  // Make it unique if it is square
  if (nb_cols == nb_rows) {
    for (uint i = 0; i < nb_rows / 2; i++) {
      game_flip_one_square(g, 2 * i, 2 * i);
      game_flip_one_square(g, 2 * i, 2 * i + 1);
      game_flip_one_square(g, 2 * i + 1, 2 * i);
      game_flip_one_square(g, 2 * i + 1, 2 * i + 1);
    }
  }
  assert(game_is_over(g));
  // apply some flips
  apply_nb_flips(g, nb_rows * nb_cols * 10);

  assert(game_is_over(g));
  // check

  game_set_some_mutable_squares(g, (nb_rows * nb_cols * 1) / 2);
  if (!with_solution) game_restart(g);
  return g;
}

/* ************************************************************************** */
