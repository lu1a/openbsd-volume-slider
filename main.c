#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/raylib.h"

#define WIDTH 300
#define HEIGHT 50
#define SCREENX 3360
#define SCREENY 2110

char* execute_command(const char *cmd) {
    FILE *fp;
    char *output = malloc(128);

    if (!output) {
        perror("malloc");
        return NULL;
    }

    fp = popen(cmd, "r");
    if (!fp) {
        perror("popen");
        free(output);
        return NULL;
    }

    if (fgets(output, 128, fp) == NULL) {
        perror("fgets");
        fclose(fp);
        free(output);
        return NULL;
    }

    fclose(fp);
    return output;
}

float read_volume() {
    float volume = 0.0f;
    char *output = execute_command("mixerctl outputs.master");
    if (output) {
        printf("Current volume: %s", output);
        int i;
        for (i = 0; i < strlen(output); i++) {
            if (output[i] == ',') {
                char temp_vol[4];
                strncpy(temp_vol, output + (i + 1), strlen(output) - (i + 1));
                volume = atof(temp_vol);
            }
        }
        free(output);
    }
    return volume;
}

void set_volume(const char *volume) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "mixerctl outputs.master=%s", volume);

    char *output = execute_command(cmd);
    if (output) {
        printf("Volume set to: %s", output);
        free(output);
    }
}

int main() {
    EnableEventWaiting();
    InitWindow(WIDTH, HEIGHT, "Volume Slider");
    SetWindowPosition(SCREENX, SCREENY);

    char temp_vol_str[8];
    float volume = read_volume();
    Rectangle track = { WIDTH / 4, HEIGHT / 2 - 10, WIDTH / 2, 20 };
    Rectangle knob = { track.x + ((volume / 255) * track.width) - 5, track.y - 5, 10, 30 };

    bool dragging = false;
    bool snapping = false;
    bool shouldRecheckSysVolume = true;

    SetTargetFPS(10);
    while (!WindowShouldClose()) {
        // Re-check sys volume every other second to ensure we're in sync
        // TODO: better method so I'm not checking GetTime AND setting a recheck var
        if ((int)GetTime() % 10 == 0) {
            if (shouldRecheckSysVolume) {
                float new_volume = read_volume();
                if ((int)volume != (int)new_volume) {
                    printf("This GUI is out of sync with system volume. Quitting.\n");
                    return 1;
                }
            }
            shouldRecheckSysVolume = false;
        } else {
            shouldRecheckSysVolume = true;
        }


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

            volume = (float)((knob.x - track.x) / (float)(track.width - knob.width)) * 255;
            int rounded_volume = (int)volume;

            // Format the result as "rounded_volume,rounded_volume"
            snprintf(temp_vol_str, sizeof(temp_vol_str), "%i,%i", (int)volume, (int)volume);
            set_volume(temp_vol_str);
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
