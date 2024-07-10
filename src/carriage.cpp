#include "carriage.h"
#include "bricks.h"
#include <GLFW/glfw3.h>

Carriage::Carriage(const ArkanoidSettings* settings, Ball* ball)
{
    this->settings = settings;
    this->ball = ball;

    size.x = settings->carriage_width;
    speed = 150;
    world_size = settings->world_size;
    left_top_point.x = (world_size.x - size.x) / 2;
    left_top_point.y = world_size.y - size.y * 2;
}

void Carriage::detect_collision(ArkanoidDebugData& debug_data)
{
    if(ball->detect_collision(left_top_point, size, debug_data))
    {
        float deviation = 0.8 * ((ball->center.x - left_top_point.x) / (size.x / 2) - 1);
        ball->direction.x = deviation;
        ball->direction.y = abs(deviation) - 1;
    }
}

bool Carriage::detect_collision(Vect left_top_point, Vect size)
{
    Vect distance = (this->left_top_point + Vect(0.5) * this->size) - (left_top_point + Vect(0.5) * size);
    if(abs(distance.x) < (this->size.x + size.x) / 2 && abs(distance.y) < (this->size.y + size.y) / 2)
    {
        return true;
    }
    return false;
}

void Carriage::update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    if (io.KeysDown[GLFW_KEY_A])
    {
        left_top_point.x -= speed * elapsed;
        if (left_top_point.x < 0)
        {
            left_top_point.x = 0;
        }
    }
    else if (io.KeysDown[GLFW_KEY_D])
    {
        left_top_point.x += speed * elapsed;
        if (left_top_point.x > world_size.x - size.x)
        {
            left_top_point.x = world_size.x - size.x;
        }
    }
}

void Carriage::draw(ImDrawList &draw_list, Vect world_to_screen)
{
    Vect lt_point = left_top_point * world_to_screen;
    Vect rb_point = (left_top_point + size) * world_to_screen;
    draw_list.AddRectFilled(lt_point, rb_point, color);
}
