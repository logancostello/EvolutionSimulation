#include "fertility.h"
#include "random.h"

const bool ALLOW_SPARSE = true;
const float SPARSE_FERTILITY = 0.02;

namespace Fertility {

    void set_world_bounds(float x, float y, float t) {
        world_size_x = x;
        world_size_y = y;
        tolerance = t;
        noise.reseed(Random::get_seed());  
    }

    std::pair<float, float> random_location() {
        constexpr float scale = 0.0006f;
        constexpr float threshold = 0.55f;

        while (true) {
            float half_x = world_size_x / 2.0f + tolerance;
            float half_y = world_size_y / 2.0f + tolerance;

            float x = Random::float_range(-half_x, half_x);
            float y = Random::float_range(-half_y, half_y);

            float fx = std::max(0.0f, std::abs(x) - world_size_x / 2.0f) / tolerance;
            float fy = std::max(0.0f, std::abs(y) - world_size_y / 2.0f) / tolerance;
            float falloff = 1.0f - std::max(fx, fy);

            float fertility = noise.normalizedOctave2D_01(x * scale, y * scale, 4, 0.5f) * falloff;

            if (fertility <= threshold) {
                if (ALLOW_SPARSE && falloff == 1) fertility = SPARSE_FERTILITY;
                else continue;
            }

            if (Random::float_range() < fertility) {
                return { x, y };
            }
        }
    }
};