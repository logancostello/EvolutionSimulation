#pragma once
#include "raylib.h"
#include <entt/entt.hpp>

class Renderer {
    public:
        void draw(entt::registry& registry);
};