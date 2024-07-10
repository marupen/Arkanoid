#pragma once

#include "arkanoid.h"
#include "bricks.h"
#include "carriage.h"
#include "stats.h"

#define USE_ARKANOID_IMPL

class ArkanoidImpl : public Arkanoid
{
public:
    void reset(const ArkanoidSettings& settings) override;
    void update(ImGuiIO& io, ArkanoidDebugData& debug_data, float elapsed) override;
    void draw(ImGuiIO& io, ImDrawList& draw_list) override;

private:
    Vect world_to_screen = Vect(0.0f);
    const ArkanoidSettings* settings;
    Bricks* bricks;
    Carriage* carriage;
    Stats* stats;
    Ball* ball;
};
