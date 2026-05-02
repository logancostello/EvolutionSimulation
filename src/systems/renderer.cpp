#include "systems/renderer.h"
#include "components/components.h"

#include "raylib.h"
#include <entt/entt.hpp>

void Renderer::draw(entt::registry& registry) {
    auto view = registry.view<Position, ColorComponent, Size>();
    for (auto [entity, pos, color, size] : view.each()) {
        DrawCircle(pos.x, pos.y, size.radius, Color{(unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, 255});
    }   
}