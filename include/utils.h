#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"

// Função para manter o HUD igual independente da resolução.
Vector2 ScaleTo720p(float x, float y, int screenWidth, int screenHeight);

// Função para manter o HUD igual independente da resolução (para Retângulos).
Rectangle ScaleRectTo720p(float x, float y, float width, float height, int screenWidth, int screenHeight);

#endif
