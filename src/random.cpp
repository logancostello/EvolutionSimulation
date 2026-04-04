#include "random.h"

namespace Random {
    std::mt19937& rng() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }

    void seed(uint32_t s) {
        rng().seed(s);
    }

    void seed() {
        std::random_device rd;
        rng().seed(rd());
    }

    int int_range(int min, int max) {
        return std::uniform_int_distribution<int>(min, max)(rng());
    }

    float float_range(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng());
    }

    float normal(float mean, float std) {
        std::normal_distribution<float> dist(mean, std);
        return dist(rng());
    }
}