#pragma once
#include "PerlinNoise.hpp"
#include <utility>

namespace Fertility {

    static siv::PerlinNoise noise;
    static float world_size_x;
    static float world_size_y;
    static float tolerance;

    void set_world_bounds(float x, float y, float t);

    std::pair<float, float> random_location();
};