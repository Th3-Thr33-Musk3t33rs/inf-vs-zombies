#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"
#include "raylib.h" // Necessário para Texture2D e outras estruturas Raylib

// Carregamento de todas as texturas do jogo.
void InitializeTextures(GameTextures* textures);

// Liberação de todas as texturas carregadas.
void UnloadTextures(GameTextures* textures);

// Renderização da tela de título.
void RenderTitleScreen(int screenWidth, int screenHeight, int fontSize);

// Renderização do HUD (Heads-Up Display: dinheiro, botão de venda).
void RenderHUD(GameState* state, int screenWidth, int screenHeight, int fontSize, 
               const GameTextures* textures, Vector2 mouse);

// Renderização do seletor de personagens na parte superior da tela.
void RenderCharacterSelector(GameState* state, int screenWidth, int screenHeight, 
                           int fontSize, const GameTextures* textures, Vector2 mouse);

// Renderização do grid principal do jogo, incluindo as tiles e personagens.
void RenderGameGrid(GameState* state, int screenWidth, int screenHeight, 
                   const GameTextures* textures, Vector2 mouse, int fontSize);

// Renderização dos projéteis ativos no jogo.
void RenderProjectiles(GameState* state, int screenWidth, int screenHeight, 
                      const GameTextures* textures);

// Renderização da bolsa de dinheiro aleatória.
void RenderPointsBag(GameState* state, int screenWidth, int screenHeight, 
                    const GameTextures* textures, Vector2 mouse);

// Renderiza o personagem selecionado ao lado do mouse de forma transparente.
void RenderSelectedCharacterPreview(GameState* state, const GameTextures* textures, Vector2 mouse, int screenWidth, int screenHeight);

#endif
