#include "fertility.h"
#include "random.h"

namespace Fertility {

    static siv::PerlinNoise noise{ (uint32_t)Random::int_range(0, INT_MAX) };

    void set_world_bounds(float x, float y) {
        world_size_x = x;
        world_size_y = y;
    }

    std::pair<float, float> random_location() {
        constexpr float scale = 0.0006f;
        constexpr float threshold = 0.5f;

        while (true) {
            float x = Random::float_range(-world_size_x / 2.0f, world_size_x / 2.0f);
            float y = Random::float_range(-world_size_y / 2.0f, world_size_y / 2.0f);

            float fertility = noise.normalizedOctave2D_01(x * scale, y * scale, 4, 0.5f);



            if (fertility > threshold && Random::float_range() < fertility) {
                return { x, y };
            }
        }
    }
};