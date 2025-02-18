#include "modules/raylib.h"

#define WIDTH 300
#define HEIGHT 50

int main() {
    InitWindow(WIDTH, HEIGHT, "Volume Slider");

    // TODO: get volume from mixerctl
    // Initial volume, out of 255
    float volume = 127.5f;
    Rectangle track = { WIDTH / 4, HEIGHT / 2 - 10, WIDTH / 2, 20 };
    Rectangle knob = { track.x + ((volume / 255) * track.width) - 5, track.y - 5, 10, 30 };

    bool dragging = false;
    bool snapping = false;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        Vector2 mousePosition = GetMousePosition();
        Rectangle mouseRect = { mousePosition.x, mousePosition.y, 1, 1 };

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionRecs(mouseRect, knob)) {
            dragging = true;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionRecs(mouseRect, knob) && CheckCollisionRecs(mouseRect, track)) {
            snapping = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            dragging = false;
            snapping = false;
        }

        if (dragging || snapping) {
            knob.x = GetMouseX() - knob.width / 2;

            // Constrain knob to track
            if (knob.x < track.x) knob.x = track.x;
            if (knob.x > track.x + track.width - knob.width) knob.x = track.x + track.width - knob.width;

            // TODO: set volume to mixerctl
            volume = (float)((knob.x - track.x) / (float)(track.width - knob.width)) * 255;
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawRectangleRec(track, GRAY);
            DrawRectangleRec(knob, DARKGRAY);

            DrawText(TextFormat("Volume: %.2f", volume), 4, 4, 8, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
