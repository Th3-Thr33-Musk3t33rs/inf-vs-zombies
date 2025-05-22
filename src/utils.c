#include "utils.h"
#include "config.h"

Vector2 ScaleTo720p(float x, float y, int screenWidth, int screenHeight) {
    float baseWidth = BASE_WIDTH_FLOAT;
    float baseHeight = BASE_HEIGHT_FLOAT;
    return (Vector2) {
        x * ((float)screenWidth / baseWidth), 
        y * ((float)screenHeight / baseHeight)
    };
}

Rectangle ScaleRectTo720p(float x, float y, float width, float height, int screenWidth, int screenHeight) {
    Vector2 pos = ScaleTo720p(x, y, screenWidth, screenHeight);
    Vector2 size = ScaleTo720p(width, height, screenWidth, screenHeight);
    return (Rectangle) {
        pos.x, pos.y, size.x, size.y
    };
}
