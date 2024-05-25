#ifndef __SPACEGAME_UI_H
#define __SPACEGAME_UI_H

#include "game_state.h"

/**
 * Draws the heads-up display of the game, containing information such as the player's
 * position, velocity, acceleration, and how many planets that has been discovered
 * 
 * @param state The current game state
 * @param planets The vector of planets
*/
void draw_hud(const GameState &state, const std::vector<Planet> &planets);

/**
 * Draws the game background grid 
 */
void draw_grid();

#endif