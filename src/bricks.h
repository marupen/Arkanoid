#pragma once

#include "ball.h"
#include "carriage.h"
#include "stats.h"
#include <vector>

enum Bonus
{
    longer_carriage,
    healing,
    fire_ball
};

struct Brick
{
    Vect left_top_point;
    int hp = 3;
};

struct Pickup
{
    Bonus bonus;
    Vect left_top_point;
    Vect size;
    float speed;
};

class Bricks
{
public:
    Bricks(const ArkanoidSettings* settings, Ball* ball, Carriage* carriage, Stats* stats);
    void detect_collision(ArkanoidDebugData& debug_data);
    void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed);
    void draw(ImDrawList &draw_list, Vect world_to_screen);

private:
    void create_bricks(float columns_padding, float rows_padding);
    void create_pickup(Vect center);

    std::vector<Brick> bricks;
    std::vector<Pickup> pickups;
    const std::vector<ImColor> bricks_colors{ImColor(255, 0, 0), ImColor(255, 255, 0), ImColor(0, 255, 0)};
    const std::vector<ImColor> pickup_colors{ImColor(255, 0, 255), ImColor(255, 255, 255), ImColor(0, 255, 0)};
    Vect size;
    Vect world_size;
    int columns_num;
    int rows_num;
    const ArkanoidSettings* settings;
    Ball* ball;
    Carriage* carriage;
    Stats* stats = nullptr;
};
