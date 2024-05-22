#ifndef __SPACEGAME_STATE_H
#define __SPACEGAME_STATE_H

#include "planet.h"
#include "splashkit.h"
#include <chrono>

#define ACCEL_RAW_MAX 200
#define ACCEL_MULT 5

struct HudInfo
{
    Planet* planet = nullptr;
    double distance = -1;
    bool intersecting = false;
};

class GameState
{
private:
    int target_fps_idx = 3;
    std::chrono::nanoseconds previous_tick;
    // In milliseconds
    double dt = 0;
    bool moving = false;

public:
    HudInfo* info = nullptr;
    vector_2d velocity = vector_2d();

    double tick();
    vector_2d calculate_acceleration() const;
    void update_velocity();
    vector_2d knockback(double normal_angle);
    void cycle_target_fps();
    bool toggle_moving();
    bool is_moving() const;
    int get_dt() const;
    int get_target_fps() const;
};

#endif