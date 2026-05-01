#include "raylib.h"
#include "rlgl.h"
#include <entt/entt.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <chrono>
#include "game_state.h"
#include "random.h"
#include "simulation.h"
#include "components/components.h"
#include "systems/renderer.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

AppState* g_state = nullptr;

int main(int argc, char* argv[]) {
    // Seed
    if (argc > 1) {
        try {
            uint32_t seed = std::stoul(argv[1]);
            Random::seed(seed);
        } catch (const std::exception&) {
            std::cerr << "Invalid seed: " << argv[1] << "\n";
            return 1;
        }
    } else {
        Random::seed();
    }
    std::cout << "Seed: " << Random::get_seed() << std::endl;

    // Headless mode: ./main <seed> <ticks>
    if (argc > 2) {
        int max_ticks = 0;
        try {
            max_ticks = std::stoi(argv[2]);
        } catch (const std::exception&) {
            std::cerr << "Invalid tick count: " << argv[2] << "\n";
            return 1;
        }

        entt::registry registry;
        auto sim = std::make_unique<Simulation>(registry);
        sim->initialize();

        const float dt = 1.0f / 60.0f;

        auto t_start = std::chrono::steady_clock::now();
        for (int tick = 0; tick < max_ticks; tick++) {
            sim->update(dt);
        }
        auto t_end = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(t_end - t_start).count();

        double checksum = 0.0;
        auto view = registry.view<Position>();
        for (auto [entity, pos] : view.each()) {
            checksum += static_cast<double>(pos.x) + static_cast<double>(pos.y);
        }

        std::cout << "Ticks:    " << max_ticks << "\n";
        std::cout << "Elapsed:  " << std::fixed << std::setprecision(3) << elapsed << "s\n";
        std::cout << "Avg/tick: " << std::fixed << std::setprecision(3) << (elapsed / max_ticks * 1000.0) << "ms\n";
        std::cout << "CHECKSUM  " << std::fixed << std::setprecision(4) << checksum << "\n";

        return 0;
    }

    // Window
    const int screen_width  = 1600;
    const int screen_height = 1000;
    const float world_width  = 7500.0f;
    const float world_height = 7500.0f;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width, screen_height, "Evolution Simulation");

    // App State
    AppState state;
    g_state = &state;

    state.sim = std::make_unique<Simulation>(state.registry);
    state.sim->initialize();

    float initial_zoom = std::min(screen_width / world_width, screen_height / world_height);
    state.camera.target = Vector2{ 0.0f, 0.0f };
    state.camera.offset = Vector2{ screen_width / 2.0f, screen_height / 2.0f };
    state.camera.zoom   = initial_zoom;

    // Main Loop
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(state.current_fps);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();
    return 0;
}