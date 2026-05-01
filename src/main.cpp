#include "raylib.h"
#include "rlgl.h"
#include <entt/entt.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <chrono>
#include "random.h"
#include "simulation.h"
#include "components/components.h"
#include "systems/renderer.h"

int main(int argc, char* argv[]) {
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

    // Normal windowed mode
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    const int screen_width = 1600;
    const int screen_height = 1000;
    InitWindow(screen_width, screen_height, "Evolution Simulation");

    const float world_width  = 7500.0f;
    const float world_height = 7500.0f;

    // Compute initial zoom so the entire world fits inside the window
    float initial_zoom = std::min(
        screen_width  / world_width,
        screen_height / world_height
    );

    Camera2D camera = {};

    camera.target = Vector2{ 0.0f, 0.0f };  
    camera.offset = Vector2{ screen_width / 2.0f, screen_height / 2.0f }; 
    camera.zoom   = initial_zoom;

    int current_fps = 60;
    SetTargetFPS(current_fps);

    entt::registry registry;
    auto sim = std::make_unique<Simulation>(registry);
    auto renderer = Renderer();

    const float dt = 1.0f / 60.0f;

    sim->initialize();

    bool paused = false;
    bool uncapped_frames = false;

    while (!WindowShouldClose()) {
        // Keyboard input
        if (IsKeyPressed(KEY_SPACE)) paused = !paused;
        else if (IsKeyPressed(KEY_F)) {
            uncapped_frames = !uncapped_frames;
            SetTargetFPS(uncapped_frames ? 0 : current_fps);
        }

        // Pan with left click
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            float zoom_scale = -1.0f / camera.zoom;
            delta = Vector2{delta.x * zoom_scale, delta.y * zoom_scale};
            camera.target = Vector2{camera.target.x + delta.x, camera.target.y + delta.y};
        }

        // Zoom with mouse wheel
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();
            camera.target = mouseWorldPos;
            float scale = 0.2f * wheel;
            camera.zoom = std::clamp(expf(logf(camera.zoom) + scale), 0.125f, 64.0f);
        }

        // Recenter camera when window resized
        if (IsWindowResized()) {
            camera.offset = Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
        }

        if (!paused) sim->update(dt);

        BeginDrawing();
            ClearBackground(Color{15, 15, 20, 255});
            BeginMode2D(camera);
                renderer.draw(registry);
            EndMode2D();
        EndDrawing();
    }
}