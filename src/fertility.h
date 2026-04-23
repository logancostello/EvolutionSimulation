#pragma once
#include "PerlinNoise.hpp"
#include <utility>

namespace Fertility {

    static float world_size_x;
    static float world_size_y;

    void set_world_bounds(float x, float y);

    std::pair<float, float> random_location();
};