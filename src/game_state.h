#pragma once
#include <entt/entt.hpp>
#include "simulation.h"
#include "systems/renderer.h"
#include "raylib.h"
#include "rlgl.h"

// App State
struct AppState {
    entt::registry registry;
    std::unique_ptr<Simulation> sim;
    Renderer renderer;
    Camera2D camera = {};
    float dt = 1.0f / 60.0f;
    int current_fps = 60;
    bool paused = false;
    bool uncapped_frames = false;
};

extern AppState* g_state;

void UpdateDrawFrame();