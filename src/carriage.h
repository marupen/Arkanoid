#pragma once

#include "ball.h"
#include <vector>

class Carriage
{
public:
    Carriage(const ArkanoidSettings* settings, Ball* ball);
    void detect_collision(ArkanoidDebugData& debug_data);
    bool detect_collision(Vect left_top_point, Vect size);
    void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);
    void draw(ImDrawList &draw_list, Vect world_to_screen);
    void increase_carriage() {size.x = settings->carriage_width * 1.25;}

private:
    Vect left_top_point;
    Vect size = Vect(0, 20);
    float speed;
    ImColor color = ImColor(0, 0, 255);
    Vect world_size;
    const ArkanoidSettings* settings;
    Ball* ball;
};
