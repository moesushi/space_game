#ifndef __SPACEGAME_PLANET_H
#define __SPACEGAME_PLANET_H

#include "splashkit.h"

class GameState;

class Planet
{
private:

public:
    double dist;
    double size;
    double radian;
    double progress = 0;
    int number;

    Planet(double distance, double size, int number);
    void probe(double dt);
    void orbit() const;
    void update(double dt);
    void draw(double dt) const;
    circle as_circle() const;
    vector_2d gravity_vector(point_2d object) const;
};

void update_planets(GameState &state, std::vector<Planet> &planets, const circle &main);
void draw_planets(double dt, const std::vector<Planet> &planets);
std::vector<Planet> generate_planets();

#endif