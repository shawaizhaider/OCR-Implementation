#include "raylib.h"

int main() {
    // Initialize the window
    InitWindow(800, 600, "raylib Test Window");

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, raylib!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    // Close the window
    CloseWindow();
    return 0;
}
