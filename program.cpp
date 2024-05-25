#include "splashkit.h"
#include "game_state.h"
#include "planet.h"
#include "ui.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <deque>
#include <vector>

void update_and_draw_trail(std::deque<point_2d> &trail)
{
    for (const point_2d &point : trail)
    {
        draw_pixel(COLOR_GREEN, point.x, point.y);
    }
    trail.push_back(screen_center());
    if (trail.size() > 1000)
    {
        trail.pop_front();
    }
}

int main()
{
    GameState state;

    load_font("default", "RobotoMono.ttf");

    open_window("Space game", 800, 800);
    current_window_toggle_fullscreen();
    display d = display_details(0);
    int x = display_width(d);
    int y = display_height(d);
    resize_current_window(x, y);

    std::deque<point_2d> trail;
    std::vector<Planet> planets = generate_planets();
    move_camera_to(-screen_width() / 2, -screen_height() / 2);

    state.tick();
    while (not quit_requested())
    {
        double dt = state.tick();
        process_events();

        if (key_typed(Q_KEY))
            return 0;
        if (key_typed(SPACE_KEY))
            state.toggle_moving();
        if (key_typed(P_KEY))
            state.cycle_target_fps();

        if (key_typed(F_KEY))
        {
            current_window_toggle_fullscreen();
            if (current_window_is_fullscreen())
            {
                display d = display_details(0);
                int x = display_width(d);
                int y = display_height(d);
                resize_current_window(x, y);
            }
            else
            {
                resize_current_window(1600, 1600);
            }
        }

        clear_screen(rgb_color(10, 10, 10));

        state.update_velocity();

        draw_grid();
        update_and_draw_trail(trail);

        move_camera_by(vector_multiply(state.velocity, dt / 1000));
        circle main = circle_at(screen_center(), 20);

        update_planets(state, planets, main);
        draw_planets(state.get_dt(), planets);

        fill_circle(COLOR_GREEN, main);

        draw_hud(state, planets);

        refresh_screen(state.get_target_fps());
    }

    return 0;
}