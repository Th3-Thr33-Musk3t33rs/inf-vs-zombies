#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"

// Texturas do jogo.
GameTextures game_textures;

// Inicialização dos gráficos do jogo.
void InitGame(void);

// Inicialização de texturas.
void InitializeTextures(GameTextures* textures);

// Liberação de texturas.
void UnloadTextures(GameTextures* textures);

// Renderização da tela de título.
void RenderTitleScreen(int screenWidth, int screenHeight, int fontSize);

// Renderização do HUD.
void RenderHUD(GameState* state, int screenWidth, int screenHeight, int fontSize, 
               const GameTextures* textures, Vector2 mouse);

// Renderização do seletor de personagens.
void RenderCharacterSelector(GameState* state, int screenWidth, int screenHeight, 
                           int fontSize, const GameTextures* textures, Vector2 mouse);

// Renderização do grid principal.
void RenderGameGrid(GameState* state, int screenWidth, int screenHeight, 
                   const GameTextures* textures, Vector2 mouse);

// Renderização de projéteis.
void RenderProjectiles(GameState* state, int screenWidth, int screenHeight, 
                      const GameTextures* textures);

// Renderização da bolsa de pontos.
void RenderPointsBag(GameState* state, int screenWidth, int screenHeight, 
                    const GameTextures* textures, Vector2 mouse);

#endif
