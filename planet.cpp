#include "planet.h"
#include "game_state.h"
#include <cmath>

Planet::Planet(double distance, double size, int number)
{
    this->dist = distance;
    this->number = number;
    this->size = size;
    this->radian = 1.0 * rnd(0, 360);
}

void Planet::probe(double dt)
{
    progress += dt / 1e4;
    if (progress > 1)
        progress = 1;
}

void Planet::orbit() const
{
    // out-of-screen culling
    double screen_distance = point_point_distance(point_at_origin(), screen_center());
    if (!(this->dist - screen_distance < screen_width() + 100))
        return;

    double precision = 0.8 / (dist / 1000);
    double angle = point_point_angle(point_2d(), screen_center());

    // render 1/16 of an orbit
    double min = angle - 12;
    double max = angle + 12;
    for (double rad = min; rad <= max; rad += precision)
    {
        vector_2d v = vector_from_angle(rad, this->dist);
        point_2d p = point_offset_from_origin(v);
        draw_pixel(rgba_color(0, 255, 255, 255), p);
    }
}

void Planet::update(double dt)
{
    auto diff = log(this->dist) / dt / 1e3;
    if (diff != INFINITY)
        radian += diff;
}

void Planet::draw() const
{
    orbit();

    circle p_circle = this->as_circle();

    if (progress == 1)
        fill_circle(COLOR_DARK_GREEN, p_circle);
    else
        draw_circle(COLOR_GRAY, p_circle);

    if (progress > 0 && progress < 1)
    {
        circle probe_circle = p_circle;
        probe_circle.radius *= progress;
        draw_circle(COLOR_GREEN, probe_circle);
    }
}

circle Planet::as_circle() const
{
    vector_2d vplanet = vector_from_angle(this->radian, this->dist);
    return circle_at(vplanet.x, vplanet.y, this->size);
}

// Similar to Newton's law of universal gravitation, with a custom gravitational constant
vector_2d Planet::gravity_vector(point_2d object) const
{
    vector_2d base_vector = vector_point_to_point(object, this->as_circle().center);
    // mass is arbitrarily just the value of radius
    double force = 50 * 20 * this->size / pow(vector_magnitude(base_vector), 2);
    return vector_multiply(unit_vector(base_vector), force);
}

void update_planets(GameState &state, std::vector<Planet> &planets, const circle &main)
{
    Planet* planet = nullptr;
    double planet_distance = -1;

    for (Planet &p : planets)
    {
        p.update(state.get_dt());

        circle p_circle = p.as_circle();
        double distance = point_point_distance(main.center, p_circle.center);
        double min_dist = main.radius + p_circle.radius;
        distance -= min_dist;

        if (distance > 10000)
            continue;

        if (planet != nullptr && distance > planet_distance)
            continue;

        planet = &p;
        planet_distance = distance;
    }

    if (planet == nullptr)
        return;

    HudInfo* info = (HudInfo*)malloc(sizeof(HudInfo));
    state.info = info;
    vector_2d gravity = planet->gravity_vector(main.center);
    state.velocity = vector_add(state.velocity, gravity);
    info->distance = planet_distance;
    info->planet = planet;
    info->intersecting = false;

    if (planet_distance <= 0)
    {
        circle p_circle = planet->as_circle();
        double angle = point_point_angle(p_circle.center, main.center);
        state.knockback(angle);

        // Reposition the player outside the planet to prevent it getting stuck
        point_2d new_bound = point_offset_by(
            planet->as_circle().center,
            vector_from_angle(angle, main.radius + p_circle.radius)
        );
        new_bound.x -= screen_width() / 2;
        new_bound.y -= screen_height() / 2;
        move_camera_to(new_bound);

        if (!state.is_moving() && vector_magnitude(state.velocity) < 50)
        {
            state.velocity = vector_limit(state.velocity, 0);
            info->intersecting = true;
            planet->probe(state.get_dt());
        }
        if (state.is_moving() && vectors_equal(state.velocity, vector_2d()) && vector_magnitude(state.calculate_acceleration()) / (ACCEL_MULT * ACCEL_RAW_MAX) >= 0.95)
            state.velocity = vector_from_angle(vector_angle(state.velocity), 100);
    }
}

void draw_planets(const std::vector<Planet> &planets)
{
    for (const Planet &planet : planets)
        planet.draw();
}

std::vector<Planet> generate_planets()
{
    std::vector<Planet> planets;
    int planet_count = rnd(2, 10);
    int previous_distance = 10000;
    for (int i = 0; i < planet_count; i++)
    {
        previous_distance += rnd(3000, 15000);
        planets.push_back(Planet(previous_distance, rnd(150, 350), i));
    }

    return planets;
}
