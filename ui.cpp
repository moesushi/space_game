#include "ui.h"
#include <cmath>

int hud_scale = 2;

void draw_hud(const GameState &state, const std::vector<Planet> &planets)
{
    drawing_options opt = option_to_screen();
    opt.scale_x = hud_scale;
    opt.scale_y = hud_scale;
    font f = font_named("default");
    int s = 12 * hud_scale;

    int probed = 0;
    for (const Planet& p : planets)
    {
        if (p.progress >= 1)
            probed += 1;
    }

    vector<string> hud_text {
        "Position: " + point_to_string(screen_center()),
        "Velocity: " + std::to_string(vector_magnitude(state.velocity)),
        "Acceleration: " + std::to_string(int(round(vector_magnitude(state.calculate_acceleration()) * 100 / (ACCEL_MULT * ACCEL_RAW_MAX)))) + "%",
        "Planets: " + std::to_string(probed) + "/" + std::to_string(planets.size()),
    };

    HudInfo* info = state.info;
    if (info != nullptr)
    {
        hud_text.push_back("Distance: " + std::to_string(std::max(0, int(info->distance))));
        if (info->intersecting)
        {
            double progress = info->planet->progress;
            if (progress == 1)
                hud_text.push_back("Probed");
            else
                hud_text.push_back("Probing: " + std::to_string(int(progress * 100)) + "%");
        }
    }

    for (int i = 0; i < hud_text.size(); i++)
    {
        string line = hud_text[i];
        draw_text(line, COLOR_WHITE, f, s, 0, s * i, opt);
    }

    point_2d centre;
    centre.x = current_window_width() / 2;
    centre.y = current_window_height() / 2;
    vector_2d vec = vector_point_to_point(centre, mouse_position());
    vec = vector_limit(vec, 200);

    int opacity = 100;
    if (!state.is_moving())
        opacity = 40;
    draw_line(rgba_color(255, 255, 255, opacity), screen_center(), point_offset_by(screen_center(), vec));
    draw_circle(rgba_color(255, 255, 255, opacity), screen_center().x, screen_center().y, vector_magnitude(vec));
}

void draw_grid()
{
    for (int x = -100000; x <= 100000; x += 500)
    {
        draw_line(rgba_color(255, 255, 255, 30), x, -100000, x, 100000);
        draw_line(rgba_color(255, 255, 255, 30), -100000, x, 100000, x);
    }
}