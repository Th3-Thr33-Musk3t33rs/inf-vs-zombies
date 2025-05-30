#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"  // Necessário para Texture2D e outras estruturas Raylib
#include "types.h"

// Carregamento de todas as texturas do jogo.
void InitializeTextures(GameTextures* textures);

// Liberação de todas as texturas carregadas.
void UnloadTextures(GameTextures* textures);

// Carregamento de todos os sons do jogo.
void InitializeSounds(GameSounds* Sounds);

// Liberação de todos sons carregadas.
void UnloadSounds(GameSounds* sounds);

void PlaySounds(GameState* state, GameSounds* sounds);

// Renderização da tela de título.
void RenderTitleScreen(int screenWidth, int screenHeight, int fontSize);

// Renderização do HUD (Heads-Up Display: dinheiro, botão de venda).
void RenderHUD(GameState* state, int screenWidth, int screenHeight, int fontSize,
               GameTextures* textures, Vector2 mouse, GameSounds* sounds);

// Renderização do seletor de personagens na parte superior da tela.
void RenderCharacterSelector(GameState* state, int screenWidth, int screenHeight,
                             int fontSize, GameTextures* textures, Vector2 mouse);

// Renderização do grid principal do jogo, incluindo as tiles e personagens.
void RenderGameGrid(GameState* state, int screenWidth, int screenHeight,
                    GameTextures* textures, Vector2 mouse, int fontSize);

// Renderização dos projéteis ativos no jogo.
void RenderProjectiles(GameState* state, int screenWidth, int screenHeight,
                       GameTextures* textures, GameSounds* sounds);

// Renderização da bolsa de dinheiro aleatória.
void RenderMoneyBag(GameState* state, int screenWidth, int screenHeight,
                    GameTextures* textures, Vector2 mouse, GameSounds* sounds);

// Renderiza o personagem selecionado ao lado do mouse de forma transparente.
void RenderSelectedCharacterPreview(GameState* state, GameTextures* textures, Vector2 mouse, int screenWidth, int screenHeight);

// Renderiza a tela de pause
void RenderPause(GameState* state, GameTextures* textures, GameSounds* sounds, Vector2 mouse, int screenWidth, int screenHeight, int fontSize);
#endif
