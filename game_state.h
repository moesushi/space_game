#ifndef __SPACEGAME_STATE_H
#define __SPACEGAME_STATE_H

#include "planet.h"
#include "splashkit.h"
#include <chrono>

#define ACCEL_RAW_MAX 200
#define ACCEL_MULT 5

/** A constant array of possible fps values, 90 by default */
const int FPS_VALUES[] = {30, 60, 75, 90, 120, 144, 160, 165, 240, 300};

/**
 * A struct containing information relevant for the HUD and some other parts of the code.
 * Mainly contains information about planet-player interactions.
 */
struct HudInfo
{
    /** A currently targeted planet. This would be the closest planet within 10000 units */
    Planet* planet = nullptr;
    /** Distance between the targeted planet and the player */
    double distance = -1;
    /** Whether or not the targeted planet and the player are intersecting (touching and thrusters disengaged) */
    bool intersecting = false;
};

class GameState
{
private:
    /** The index of the target frame rate, 90 by default */
    int target_fps_idx = 3;
    /** The time of the last tick */
    std::chrono::nanoseconds previous_tick;
    /** The result of the last call to tick(), which is the time elapsed since the previous tick in milliseconds */
    double dt = 0;
    /** Whether or not the thrusters are engaged, allowing acceleration and change of velocity */
    bool moving = false;

public:
    /** A pointer to the HudInfo struct */
    HudInfo* info = nullptr;

    /** The current vector of velocity of the player */
    vector_2d velocity = vector_2d();

    /**
     * Ticks the game state forward
     * 
     * @returns The time elapsed since the previous tick in milliseconds
     */
    double tick();

    /**
     * Calculates the acceleration vector, which depends on the location of the mouse relative to the player/screen centre
     * 
     * @returns The acceleration vector
     */
    vector_2d calculate_acceleration() const;

    /**
     * Updates the players' velocity by applying acceleration and limits
     */
    void update_velocity();

    /**
     * Performs a bounce collision
     * 
     * @param normal_angle The angle of the player to the tangent at the point of collision
     * 
     * @returns The newly updated velocity, after applying the change of angle and halving its velocity
     */
    vector_2d knockback(double normal_angle);

    /**
     * Cycles the target fps to the next one
     */
    void cycle_target_fps();

    /** 
     * Toggles the state of the thrusters
     * 
     * @returns The new state of the thrusters
     */
    bool toggle_moving();

    /** 
     * Gets whether or not the thrusters are engaged, allowing acceleration and change of velocity 
     * 
     * @returns true if thrusters are engaged, false otherwise
     */
    bool is_moving() const;

    /**
     * Gets the time elapsed since the previous tick
     * 
     * @returns The time elapsed since the previous tick in milliseconds
     */
    int get_dt() const;

    /**
     * Gets the target frame rate
     * 
     * @returns The target frame rate in fps
     */
    int get_target_fps() const;
};

#endif