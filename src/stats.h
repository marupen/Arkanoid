#pragma once

#include "arkanoid.h"

class Stats
{
public:
    Stats(const ArkanoidSettings* settings, Arkanoid* arkanoid);
    void draw(ImDrawList &draw_list, Vect world_to_screen);
    void add_hp(int hp);
    void add_points(int points) {this->points += points;}
    void win();
    void end_game_screen(ImDrawList &draw_list, Vect world_to_screen);
    void update_hight_score();

    bool is_fire_ball = false;
    bool is_game_over = false;

private:
    int hp = 2;
    int points = 0;
    int hight_score = 0;

    ImColor color = ImColor(0, 0, 255);
    float text_height;
    Vect size_hp_points;
    const ArkanoidSettings* settings;
    Arkanoid* arkanoid;
};
