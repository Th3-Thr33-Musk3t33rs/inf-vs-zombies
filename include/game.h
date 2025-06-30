#ifndef GAME_H
#define GAME_H

#include "types.h"

// InitGame inicializa os gráficos e estado do jogo.
void InitGame(GameState *state, GameTextures *textures, GameSounds *sounds);

// InitializeGameState inicializa o estado do jogo com valores padrão.
void InitializeGameState(GameState *state, bool reset);

// UpdateGame atualiza a lógica principal do jogo (chamada por frame).
void UpdateGame(GameState *state, float deltaTime);

// ProcessGameInput processa a entrada do usuário.
void ProcessGameInput(GameState *state, Vector2 mousePos, GameSounds *sounds);

// UpdateCharacters atualiza os estados e animações dos personagens.
void UpdateCharacters(GameState *state, float deltaTime);

// UpdateProjectiles atualiza a lógica dos projéteis.
void UpdateProjectiles(GameState *state, float deltaTime);

// UpdateBombs atualiza a lógica das bombas.
void UpdateBombs(GameState *state, float deltaTime);

// UpdateMoneyBag atualiza a lógica da bolsa de dinheiro.
void UpdateMoneyBag(GameState *state, float deltaTime);

// HandleCharacterPlacement lida com a lógica de posicionamento de personagens no grid.
void HandleCharacterPlacement(GameState *state, int row, int col);

// HandleCharacterSelling lida com a lógica de venda de personagens no grid.
void HandleCharacterSelling(GameState *state, int row, int col);

// HandleCharacterInteraction lida com a lógica de interações com os personagens no grid.
void HandleCharacterInteractions(GameState *state, int row, int col);

// HandlePauseMenu lida com a lógica do menu de pause.
void HandlePauseMenu(GameState *state, Vector2 mousePos, GameSounds *sounds);

// HandlePauseMenu lida com a lógica dos botões do menu de leaderboard.
void HandleLeaderboardMenu(GameState *state, Vector2 mousePos);

// Coloca um zumbi no início do caminho.
void SpawnZombie(GameState *state);

// UpdateHordeLogic controla o spawn de zumbis.
void UpdateHordeLogic(GameState *state, float deltaTime);
#endif
