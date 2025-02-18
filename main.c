#include "modules/raylib.h"

#define WIDTH 800
#define HEIGHT 400
#define HEADER_FONT_SIZE 50

int main()
{
    InitWindow(WIDTH, HEIGHT, "Volume Control");
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(LIME);
            DrawText("brat", (WIDTH/2)-HEADER_FONT_SIZE, (HEIGHT/2)-HEADER_FONT_SIZE, HEADER_FONT_SIZE, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
