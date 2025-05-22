#ifndef GAME_H
#define GAME_H

#include "types.h"

// Estado do jogo.
GameState game_state;

// Inicialização do estado do jogo.
void InitializeGameState(GameState* state);

// Atualização da lógica do jogo.
void UpdateGame(GameState* state);

// Atualização dos personagens.
// void UpdateCharacters(GameState* state);

// // Processamento de entrada do usuário.
// void ProcessInput(GameState* state, Vector2 mousePos, int screenWidth, int screenHeight);

// // Lógica de posicionamento de personagens.
// void PlaceCharacter(GameState* state, int row, int col);

// // Lógica de venda de personagens
// void SellCharacter(GameState* state, int row, int col);

// // Verificação de colisão com tiles
// bool CheckTileCollision(Vector2 mousePos, int row, int col, int screenWidth, int screenHeight);

#endif
