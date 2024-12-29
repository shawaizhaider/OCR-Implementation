#include "../../include/utils/GUI_utils.hpp"
#include <raylib.h>
#include <iostream>
#include <math.h>
void ShowTerminalProgress(const std::string& message) {
    static int dots = 0;
    std::cout << "\r" << message;
    for(int i = 0; i < dots; i++) std::cout << ".";
    for(int i = dots; i < 3; i++) std::cout << " ";
    std::cout.flush();
    dots = (dots + 1) % 4;
}

void DrawLoadingSpinner(float posX, float posY) {
    static float rotation = 0.0f;
    rotation += 8.0f;  // Faster rotation
    
    const int numDots = 12;
    const float radius = 30.0f;
    
    for (int i = 0; i < numDots; i++) {
        float angle = rotation * DEG2RAD + ((float)i * 2 * PI) / numDots;
        float alpha = 1.0f - ((float)i / numDots);
        float dotX = posX + cosf(angle) * radius;
        float dotY = posY + sinf(angle) * radius;
        DrawCircle((int)dotX, (int)dotY, 4, Fade(BLACK, alpha));
    }
}
