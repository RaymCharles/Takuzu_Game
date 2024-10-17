#include "game.h"
#include "queue.h"

/**
 * @brief The structure pointer that stores the game state.
 **/
typedef struct game_s {
  square* tab;
  uint size_column;  // (y ou i dans le jeu)
  uint size_line;    // (x ou j dans le jeu)
  bool wrapping;
  bool unique;
  queue* histo_played;
  queue* histo_canceled;
} game_s;

/*
(0,0) (0,1) (0,2) (0,3) (0,4) (1,0) (1,1) (1,2) (1,3) (1,4) (2,0) (2,1) (2,2)
(2,3)* (2,4) ...

  0 1 2 3 4 --- j
0       |
1       |
2 ------*
3
4
|
i

Pour accÃ©der Ã  la case * : i*taille de ligne + j <=> i*(g->size_ligne) + j
Ici, on veut accÃ©der au point (2,3) : 2*5 + 3 = 13
*/

typedef struct datamove {
  uint pos_i;
  uint pos_j;
  square sq;
} datamove;