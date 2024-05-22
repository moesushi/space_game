#ifndef __SPACEGAME_UI_H
#define __SPACEGAME_UI_H

#include "game_state.h"

void draw_hud(const GameState &state, const std::vector<Planet> &planets);
void draw_grid();

#endif