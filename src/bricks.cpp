#include "bricks.h"
#include <iostream>

Bricks::Bricks(const ArkanoidSettings* settings, Ball* ball, Carriage* carriage, Stats* stats)
{
    this->settings = settings;
    this->ball = ball;
    this->carriage = carriage;
    this->stats = stats;

    columns_num = settings->bricks_columns_count;
    rows_num = settings->bricks_rows_count;
    world_size = settings->world_size;

    create_bricks(settings->bricks_columns_padding, settings->bricks_rows_padding);
}

void Bricks::create_bricks(float columns_padding, float rows_padding)
{
    size.x = (world_size.x - columns_padding) / static_cast<float>(columns_num) - columns_padding;
    size.y = (world_size.y / 2 - rows_padding) / static_cast<float>(rows_num) - rows_padding;

    Vect point = Vect(columns_padding, world_size.y / 2 - size.y - rows_padding + world_size.y * 0.1);

    for(int i = 0; i < rows_num; i++)
    {
        for(int j = 0; j < columns_num; j++)
        {
            Brick brick;
            brick.left_top_point = Vect(point);
            bricks.push_back(brick);

            point.x += size.x + columns_padding;
        }

        point.x = columns_padding;
        point.y -= size.y + rows_padding;
    }
}

void Bricks::detect_collision(ArkanoidDebugData& debug_data)
{

    for(int i = 0; i < bricks.size(); i++)
    {
        if(ball->detect_collision(bricks[i].left_top_point, size, debug_data))
        {
            if(stats->is_fire_ball) bricks[i].hp = 0;
            else bricks[i].hp--;
            if(bricks[i].hp <= 0)
            {
                float probability = mathfu::RandomInRange<float>(0, 1);
                if(probability < 0.1) create_pickup(bricks[i].left_top_point + Vect(0.5) * size);
                bricks.erase(bricks.begin() + i);
                if(stats)
                {
                    stats->add_points(10);
                }
                if(bricks.empty())
                {
                    stats->win();
                }
            }
        }
    }

    for(int i = 0; i < pickups.size(); i++)
    {
        if(carriage->detect_collision(pickups[i].left_top_point, pickups[i].size))
        {
            switch (pickups[i].bonus) {
            case longer_carriage:
                carriage->increase_carriage();
                break;
            case healing:
                if(stats) stats->add_hp(1);
                break;
            case fire_ball:
                ball->set_color(ImColor(255, 0, 0));
                stats->is_fire_ball = true;
                break;
            }
            pickups.erase(pickups.begin() + i);
        }
    }
}

void Bricks::create_pickup(Vect center)
{
    Pickup pickup;
    pickup.size = Vect(0.5) * size;
    pickup.left_top_point = center - Vect(0.5) * pickup.size;
    pickup.speed = settings->ball_speed;
    pickup.bonus = Bonus(mathfu::RandomInRange<int>(0, 3));
    pickups.push_back(pickup);
}

void Bricks::update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed)
{
    for(int i = 0; i < pickups.size(); i++)
    {
        pickups[i].left_top_point.y += pickups[i].speed * elapsed;
        if(pickups[i].left_top_point.y > world_size.y) pickups.erase(pickups.begin() + i);
    }
}

void Bricks::draw(ImDrawList &draw_list, Vect world_to_screen)
{
    for(Brick brick : bricks)
    {
        Vect left_top_point = brick.left_top_point * world_to_screen;
        Vect right_bottom_point = (brick.left_top_point + size) * world_to_screen;
        draw_list.AddRectFilled(left_top_point, right_bottom_point, bricks_colors[brick.hp - 1]);
    }

    for(Pickup pickup : pickups)
    {
        Vect left_top_point = pickup.left_top_point * world_to_screen;
        Vect right_bottom_point = (pickup.left_top_point + pickup.size) * world_to_screen;
        draw_list.AddRectFilled(left_top_point, right_bottom_point, pickup_colors[pickup.bonus]);
    }
}
