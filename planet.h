#ifndef __SPACEGAME_PLANET_H
#define __SPACEGAME_PLANET_H

#include "splashkit.h"

class GameState; // A forward declaration; actual declaration is in game_state.h

/**
 * Represents a planet in the game
 */
class Planet
{
private:
    /** The distance of the planet from the origin */
    double dist;
    /** The radius of the planet */
    double size;
    /** The angle of the planet from the origin */
    double radian;

    /**
     * Draws the planet's orbit
     */
    void orbit() const;

public:
    /** The probing progress, expressed from 0 to 1 */
    double progress = 0;
    /** The planet's number, this successively increases for every next planet from the origin */
    int number;

    /**
     * Construct a Planet
     * 
     * @param distance The distance of the planet from the origin
     * @param size The radius of the planet
     * @param number The planet's number/index
     */
    Planet(double distance, double size, int number);

    /**
     * Progress probing the planet
     * 
     * @param dt The time elapsed since the previous tick, in milliseconds
     */
    void probe(double dt);

    /**
     * Updates and performs movement on the planet
     * 
     * @param dt The time elapsed since the previous tick, in milliseconds
     */
    void update(double dt);

    /**
     * Draws the planet to the screen
     */
    void draw() const;

    /**
     * Gets a circle representing the planet
     * 
     * @returns A circle of the planet
     */
    circle as_circle() const;

    /**
     * Gets a vector of the planets' force of gravity upon an object
     * 
     * @param object The object to calculate gravity against, usually always the player
     * 
     * @returns A vector, representing the force the planet exerts against an object
    */
    vector_2d gravity_vector(point_2d object) const;
};

/**
 * Updates all planets
 * 
 * @param state The current game state
 * @param planets The vector of planets
 * @param main The player's main circle in the middle of the screen
 */
void update_planets(GameState &state, std::vector<Planet> &planets, const circle &main);

/**
 * Draws all planets
 *
 * @param planets The vector of planets
 */
void draw_planets(const std::vector<Planet> &planets);

/**
 * Generates a vector of planets
 * 
 * @returns The vector of generated planets
 */
std::vector<Planet> generate_planets();

#endif