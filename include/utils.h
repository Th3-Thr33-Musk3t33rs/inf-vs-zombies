#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include "raylib.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

// Função para manter o HUD igual independente da resolução.
Vector2 ScaleTo720p(float x, float y, int screenWidth, int screenHeight);

// Função para manter o HUD igual independente da resolução (para Retângulos).
Rectangle ScaleRectTo720p(float x, float y, float width, float height, int screenWidth, int screenHeight);

void SaveLeaderboard(char *fileName, PlayerLeaderboard *leaderboard);

PlayerLeaderboard* LoadLeaderboard(char *fileName);

// Transforma um determinado tempo (em segundos) no número de quadros que o jogo exibe nesse intervalo.
int TimeToFrames(float timeInSeconds);
#endif
