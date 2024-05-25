#include "game_state.h"

double GameState::tick()
{
    std::chrono::nanoseconds current_tick = std::chrono::system_clock::now().time_since_epoch();
    using ms = std::chrono::duration<double, std::milli>;
    ms ndt = current_tick - previous_tick;
    previous_tick = current_tick;
    dt = ndt.count();

    if (info != nullptr)
    {
        free(info);
        info = nullptr;
    }

    return dt;
}

vector_2d GameState::calculate_acceleration() const
{
    point_2d centre;
    centre.x = current_window_width() / 2;
    centre.y = current_window_height() / 2;
    vector_2d accel_vec = vector_point_to_point(centre, mouse_position());
    accel_vec = vector_limit(accel_vec, ACCEL_RAW_MAX);
    accel_vec = vector_multiply(accel_vec, ACCEL_MULT);
    return accel_vec;
}

void GameState::update_velocity()
{
    if (!moving)
        return;

    if (info != nullptr && info->intersecting)
        return;

    vector_2d accel_vec = calculate_acceleration();
    accel_vec = vector_multiply(accel_vec, dt / 1000);
    velocity = vector_add(velocity, accel_vec);
    velocity = vector_limit(velocity, 5000);
}

vector_2d GameState::knockback(double normal_angle)
{
    double current_angle = vector_angle(this->velocity);
    double resulting_angle = 2 * (normal_angle - 90) - current_angle; // https://stackoverflow.com/a/64421233
    return this->velocity = vector_from_angle(resulting_angle, vector_magnitude(this->velocity) * 0.5); // 50% vel lost on impact
}

void GameState::cycle_target_fps()
{
    if (target_fps_idx >= 9)
    {
        target_fps_idx = -1;
    }
    target_fps_idx += 1;
}

bool GameState::toggle_moving()
{
    return this->moving = !this->moving;
}

bool GameState::is_moving() const
{
    return this->moving;
}

int GameState::get_dt() const
{
    return dt;
}

int GameState::get_target_fps() const
{
    return FPS_VALUES[target_fps_idx];
}