#pragma once

#include "arkanoid.h"
#include "stats.h"

class Ball
{
public:
    Ball(const ArkanoidSettings* settings, Stats* stats);
    bool detect_collision(Vect left_top_point, Vect size, ArkanoidDebugData& debug_data);
    void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);
    void draw(ImDrawList &draw_list, Vect world_to_screen);
    void set_color(ImColor color) {this->color = color;}
    Vect center;
    Vect direction;

private:
    void add_debug_hit(ArkanoidDebugData& debug_data, const Vect& world_pos, const Vect& normal);

    Vect world_to_screen;
    float radius;
    float speed;
    const ArkanoidSettings* settings;
    ImColor color = ImColor(255, 255, 255);
    Stats* stats;
};
