/**
 * \file game_aux.h
 * \brief Auxiliarry Game Functions.
 * \details See \ref index for further details.
 * \copyright University of Bordeaux. All rights reserved, 2021.
 **/

#ifndef __GAME_AUX_H__
#define __GAME_AUX_H__
#include <stdbool.h>

#include "game.h"

// ---------------------------------------------------------------------------------------------
//   * * * * *  GAME_PRINT  * * * * *
// ---------------------------------------------------------------------------------------------
/**
 * \brief Prints a game as text on the standard output stream.
 * \details The different squares are respectively displayed as text, as
 * described in \ref index.
 * \param g the game
 * \pre \p g must be a valid pointer toward a game structure.
 **/
void game_print(cgame g);

// ---------------------------------------------------------------------------------------------
//   * * * * *  GAME_DEFAULT  * * * * *
// ---------------------------------------------------------------------------------------------
/**
 * \brief Creates the default game.
 * \details See the description of the default game in \ref index.
 * \return the created game
 **/
game game_default(void);

// ---------------------------------------------------------------------------------------------
//   * * * * *  GAME_DEFAULT_SOLUTION  * * * * *
// ---------------------------------------------------------------------------------------------
/**
 * \brief Creates the default game solution.
 * \details See the description of the default game in \ref index.
 * \return the created game
 **/
game game_default_solution(void);

#endif  // __GAME_AUX_H__
