#include "stats.h"
#include <string>
#include <iostream>
#include <fstream>

Stats::Stats(const ArkanoidSettings* settings, Arkanoid* arkanoid)
{
    this->settings = settings;
    this->arkanoid = arkanoid;

    text_height = settings->world_size.y * 0.08;
    size_hp_points = Vect(settings->world_size.x * 0.25, text_height);
}

void Stats::draw(ImDrawList &draw_list, Vect world_to_screen)
{
    std::string points_string = "Points: " + std::to_string(points);
    draw_list.AddText(NULL, text_height * world_to_screen.y, Vect(0, 0), color, &points_string[0], &points_string[points_string.length()]);

    Vect left_top_point = Vect(settings->world_size.x, settings->world_size.y * 0.01);
    for(int i = 0; i < hp; i++)
    {
        left_top_point.x -= size_hp_points.x + settings->world_size.x * 0.01;
        Vect rb_point_scaled = (left_top_point + size_hp_points) * world_to_screen;
        Vect lt_point_scaled = left_top_point * world_to_screen;
        draw_list.AddRectFilled(lt_point_scaled, rb_point_scaled, color);
    }
}

void Stats::add_hp(int hp)
{
    this->hp += hp;
    if(this->hp < 0)
    {
        update_hight_score();
        is_game_over = true;
    }
    else if(this->hp > 2) this->hp = 2;
}

void Stats::win()
{
    update_hight_score();
    is_game_over = true;
}

void Stats::end_game_screen(ImDrawList &draw_list, Vect world_to_screen)
{
    float text_height = settings->world_size.y * 0.1 * world_to_screen.y;
    std::string info = "GAME OVER\n\nYOUR SCORE: ";
    info.append(std::to_string(points));
    info.append("\n\nHIGH SCORE: ");
    info.append(std::to_string(hight_score));
    info.append("\n\npress SPACE to restart");

    draw_list.AddText(NULL, text_height, Vect(text_height), color, &info[0], &info[info.length()]);
}

void Stats::update_hight_score()
{
    std::string line;

    std::ifstream in("hight_score.txt");
    if (in.is_open())
    {
        if (std::getline(in, line))
        {
            if('0' <= line[0] && line[0] <= '9')
            {
                hight_score = std::stoi(line);
            }
        }
    }
    in.close();

    if(points > hight_score)
    {
        hight_score = points;
        std::ofstream out;
        out.open("hight_score.txt");
        if (out.is_open())
        {
            out << hight_score << std::endl;
        }
        out.close();
    }
}
