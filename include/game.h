#ifndef GAME_H
#define GAME_H

#include "types.h"

// Inicialização dos gráficos e estado do jogo (configurações da janela, FPS, lógica inicial...).
void InitGame(GameState* state, GameTextures* textures);

// Inicialização do estado do jogo.
void InitializeGameState(GameState* state);

// Atualização da lógica principal do jogo (chamada por frame).
void UpdateGame(GameState* state);

// Processamento de entrada do usuário que afeta o estado do jogo.
void ProcessGameInput(GameState* state, Vector2 mousePos, int screenWidth, int screenHeight);

// Atualização dos estados e animações dos personagens.
void UpdateCharacters(GameState* state);

// Atualização da lógica dos projéteis.
void UpdateProjectiles(GameState* state, float deltaTime);

// Atualização da lógica da bolsa de pontos aleatória.
void UpdateMoneyBag(GameState* state);

// Lógica de posicionamento e venda de personagens no grid.
void HandleCharacterPlacementAndSelling(GameState* state, Vector2 mouse, int screenWidth, int screenHeight);

#endif
