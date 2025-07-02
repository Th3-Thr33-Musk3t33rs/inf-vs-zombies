#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "raylib.h"
#include "types.h"

// Função para manter o HUD igual independente da resolução (para Textos).
Vector2 ScaleTo720p(float x, float y, int screenWidth, int screenHeight);

// Função para manter o HUD igual independente da resolução (para Retângulos).
Rectangle ScaleRectTo720p(float x, float y, float width, float height, int screenWidth, int screenHeight);

// SaveLeaderboard é uma função que escreve no arquivo que contém o ranking de jogadores.
void SaveLeaderboard(char *fileName, GameState *gameState);

// LoadLeaderboard é uma função que lê o arquivo que contém o ranking de jogadores.
void LoadLeaderboard(char *fileName, GameState *gamestate);

// TimeToFrames transforma um determinado tempo (em segundos) no número de quadros que o jogo exibe nesse intervalo.
int TimeToFrames(float timeInSeconds);
#endif
