#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"
#include "raylib.h" // Necessário para Texture2D e outras estruturas Raylib

// Declaração da variável de texturas do jogo.
// Ela será definida em main.c.
extern GameTextures game_textures;

// Inicialização dos gráficos do jogo (configurações da janela, FPS).
void InitGame(void);

// Carregamento de todas as texturas do jogo.
void InitializeTextures(GameTextures* textures);

// Liberação de todas as texturas carregadas.
void UnloadTextures(GameTextures* textures);

// Renderização da tela de título.
void RenderTitleScreen(int screenWidth, int screenHeight, int fontSize);

// Renderização do HUD (Heads-Up Display: pontos, botão de venda).
void RenderHUD(GameState* state, int screenWidth, int screenHeight, int fontSize, 
               const GameTextures* textures, Vector2 mouse);

// Renderização do seletor de personagens na parte superior da tela.
void RenderCharacterSelector(GameState* state, int screenWidth, int screenHeight, 
                           int fontSize, const GameTextures* textures, Vector2 mouse);

// Renderização do grid principal do jogo, incluindo as tiles e personagens.
void RenderGameGrid(GameState* state, int screenWidth, int screenHeight, 
                   const GameTextures* textures, Vector2 mouse);

// Renderização dos projéteis ativos no jogo.
void RenderProjectiles(GameState* state, int screenWidth, int screenHeight, 
                      const GameTextures* textures);

// Renderização da bolsa de pontos aleatória.
void RenderPointsBag(GameState* state, int screenWidth, int screenHeight, 
                    const GameTextures* textures, Vector2 mouse);

// Renderiza o personagem selecionado ao lado do mouse de forma transparente.
void RenderSelectedCharacterPreview(GameState* state, const GameTextures* textures, Vector2 mouse, int screenWidth, int screenHeight);

#endif
