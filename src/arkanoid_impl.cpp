#include "arkanoid_impl.h"
#include <GLFW/glfw3.h>

#ifdef USE_ARKANOID_IMPL
Arkanoid* create_arkanoid()
{
    return new ArkanoidImpl();
}
#endif

void ArkanoidImpl::reset(const ArkanoidSettings &settings)
{
    this->settings = &settings;
    stats = new Stats(&settings, this);
    ball = new Ball(&settings, stats);
    carriage = new Carriage(&settings, ball);
    bricks = new Bricks(&settings, ball, carriage, stats);
}

void ArkanoidImpl::update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    if(stats && !stats->is_game_over)
    {
        ball->update(io, debug_data, elapsed);
        carriage->update(io, debug_data, elapsed);
        bricks->update(io, debug_data, elapsed);
        carriage->detect_collision(debug_data);
        bricks->detect_collision(debug_data);
    }
    else if(io.KeysDown[GLFW_KEY_SPACE])
    {
        delete bricks;
        delete carriage;
        delete stats;
        delete ball;
        reset(*settings);
    }
}

void ArkanoidImpl::draw(ImGuiIO& io, ImDrawList &draw_list)
{
    world_to_screen = Vect(io.DisplaySize.x / settings->world_size.x, io.DisplaySize.y / settings->world_size.y);

    if(stats)
    {
        if(!stats->is_game_over)
        {
            bricks->draw(draw_list, world_to_screen);
            carriage->draw(draw_list, world_to_screen);
            ball->draw(draw_list, world_to_screen);
            stats->draw(draw_list, world_to_screen);
        }
        else
        {
            stats->end_game_screen(draw_list, world_to_screen);
        }
    }
}
