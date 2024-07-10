#include "ball.h"

Ball::Ball(const ArkanoidSettings* settings, Stats* stats)
{
    this->settings = settings;
    this->stats = stats;

    center = Vect(settings->world_size.x * 0.5, settings->world_size.y * 0.75f);
    radius = settings->ball_radius;
    direction = Vect(1, 1).Normalized();
    speed = settings->ball_speed;
}

bool Ball::detect_collision(Vect left_top_point, Vect size, ArkanoidDebugData& debug_data)
{
    if(abs(center.x - (left_top_point.x + size.x / 2)) < (radius + size.x / 2)
        && abs(center.y - (left_top_point.y + size.y / 2)) < (radius + size.y / 2))
    {
        float delta_x = 0.0f;
        float delta_y = 0.0f;

        if(direction.x > 0)
        {
            delta_x = center.x + radius - left_top_point.x;
        }
        else
        {
            delta_x = left_top_point.x + size.x - (center.x - radius);
        }

        if(direction.y > 0)
        {
            delta_y = center.y + radius - left_top_point.y;
        }
        else
        {
            delta_y = left_top_point.y + size.y - (center.y - radius);
        }

        Vect collision_pos;
        Vect collision_normal;

        if(delta_x > delta_y)
        {
            direction.y *= -1;
            if(direction.y < 0)
            {
                center.y = left_top_point.y - radius;
                collision_pos = Vect(center.x, center.y + radius);
                collision_normal = Vect(0, -1);
            }
            else
            {
                center.y = left_top_point.y + size.y + radius;
                collision_pos = Vect(center.x, center.y - radius);
                collision_normal = Vect(0, 1);
            }
        }
        else
        {
            direction.x *= -1;
            if(direction.x < 0)
            {
                center.x = left_top_point.x - radius;
                collision_pos = Vect(center.x + radius, center.y);
                collision_normal = Vect(-1, 0);
            }
            else
            {
                center.x = left_top_point.x + size.x + radius;
                collision_pos = Vect(center.x - radius, center.y);
                collision_normal = Vect(1, 0);
            }
        }

        add_debug_hit(debug_data, collision_pos, collision_normal);

        return true;
    }
    return false;
}

void Ball::update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    world_to_screen = Vect(io.DisplaySize.x / settings->world_size.x, io.DisplaySize.y / settings->world_size.y);

    center += direction * speed * elapsed;

    if(center.x < radius)
    {
        center.x += (radius - center.x) * 2.0f;
        direction.x *= -1.0f;

        add_debug_hit(debug_data, Vect(0, center.y), Vect(1, 0));
    }
    else if(center.x > (settings->world_size.x - radius))
    {
        center.x -= (center.x - (settings->world_size.x - radius)) * 2.0f;
        direction.x *= -1.0f;

        add_debug_hit(debug_data, Vect(settings->world_size.x, center.y), Vect(-1, 0));
    }

    if(center.y < radius)
    {
        center.y += (radius - center.y) * 2.0f;
        direction.y *= -1.0f;

        add_debug_hit(debug_data, Vect(center.x, 0), Vect(0, 1));
    }
    else if(center.y > (settings->world_size.y + radius))
    {
        add_debug_hit(debug_data, Vect(center.x, settings->world_size.y), Vect(0, -1));

        center = Vect(settings->world_size.x * 0.5, settings->world_size.y * 0.75f);
        direction = Vect(1, 1).Normalized();
        stats->add_hp(-1);
        stats->is_fire_ball = false;
        set_color(ImColor(255, 255, 255));
    }
}

void Ball::draw(ImDrawList &draw_list, Vect world_to_screen)
{
    Vect screen_pos = center * world_to_screen;
    float screen_radius = radius * world_to_screen.x;
    draw_list.AddCircleFilled(screen_pos, screen_radius, color);
}

void Ball::add_debug_hit(ArkanoidDebugData& debug_data, const Vect& world_pos, const Vect& normal)
{
    ArkanoidDebugData::Hit hit;
    hit.screen_pos = world_pos * world_to_screen;
    hit.normal = normal;
    debug_data.hits.push_back(std::move(hit));
}
