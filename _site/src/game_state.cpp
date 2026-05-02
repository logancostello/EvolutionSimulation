#include "game_state.h"
#include "simulation.h"
#include "raylib.h"
#include "rlgl.h"

void UpdateDrawFrame() {
    AppState& s = *g_state;

    // Input Loop
    if (IsKeyPressed(KEY_SPACE)) s.paused = !s.paused;
    else if (IsKeyPressed(KEY_F)) {
        s.uncapped_frames = !s.uncapped_frames;
        SetTargetFPS(s.uncapped_frames ? 0 : s.current_fps);
    }

    // Pan with left click
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 delta = GetMouseDelta();
        float zoom_scale = -1.0f / s.camera.zoom;
        delta = Vector2{delta.x * zoom_scale, delta.y * zoom_scale};
        s.camera.target = Vector2{s.camera.target.x + delta.x, s.camera.target.y + delta.y};
    }

    // Zoom with mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), s.camera);
        s.camera.offset = GetMousePosition();
        s.camera.target = mouseWorldPos;
        float scale = 0.2f * wheel;
        s.camera.zoom = std::clamp(expf(logf(s.camera.zoom) + scale), 0.125f, 64.0f);
    }

    // Recenter camera when window resized
    if (IsWindowResized()) {
        s.camera.offset = Vector2{ (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
    }

    // Update
    if (!s.paused) s.sim->update(s.dt);

    // Draw
    BeginDrawing();
        ClearBackground(Color{15, 15, 20, 255});
        BeginMode2D(s.camera);
            s.renderer.draw(s.registry);
        EndMode2D();
    EndDrawing();
}