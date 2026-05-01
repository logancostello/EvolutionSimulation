#include "systems/renderer.h"
#include "components/components.h"

#include "raylib.h"
#include <entt/entt.hpp>

void Renderer::draw(entt::registry& registry) {
    auto view = registry.view<Position, ColorComponent, Size>();
    for (auto [entity, pos, color, size] : view.each()) {
        DrawCircle(pos.x, pos.y, size.radius, Color{color.r, color.g, color.b, 255});
    }   
}