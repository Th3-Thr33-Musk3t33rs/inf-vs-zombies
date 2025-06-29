#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include "types.h"

// InitializeTextures carrega todas as texturas do jogo.
void InitializeTextures(GameTextures *textures);

// UnloadTextures libera todas as texturas carregadas.
void UnloadTextures(GameTextures *textures);

// InitializeSounds carrega todos os sons do jogo.
void InitializeSounds(GameSounds *Sounds);

// UnloadSounds libera todos os sons carregados.
void UnloadSounds(GameSounds *sounds);

// PlaySounds toca os sons dados como entrada.
void PlaySounds(GameState *state, GameSounds *sounds);

// RenderTitleScreen renderiza a tela de título.
void RenderTitleScreen(int screenWidth, int screenHeight, int fontSize, GameState *state, GameTextures *textures, Vector2 mouse);

// RenderHUD renderiza o HUD.
void RenderHUD(GameState *state, GameTextures *textures, Vector2 mouse);

// RenderCharacterSelector renderiza o seletor de personagens.
void RenderCharacterSelector(GameState *state, GameTextures *textures, Vector2 mouse);

// RenderStatsPanel renderiza o painel de estatísticas.
void RenderStatsPanel(PlayerStats *stats, GameTextures *textures);

// RenderGameGrid renderiza o grid principal do jogo, incluindo tiles e personagens.
void RenderGameGrid(GameState *state, GameTextures *textures, Vector2 mouse);

// RenderProjectiles renderiza os projéteis ativos no jogo.
void RenderProjectiles(GameState *state, GameTextures *textures);

// RenderBombs renderiza as bombas ativas no jogo.
void RenderBombs(GameState *state, GameTextures *textures);

// RenderMoneyBag renderiza a bolsa de dinheiro aleatória.
void RenderMoneyBag(GameState *state, GameTextures *textures, Vector2 mouse);

// RenderSelectedCharacterPreview renderiza  o personagem selecionado ao lado do mouse.
void RenderSelectedCharacterPreview(GameState *state, GameTextures *textures, Vector2 mouse);

// RenderPause renderiza a tela de pause.
void RenderPause(GameState *state, GameTextures *textures, Vector2 mouse);

// RenderLeaderboard renderiza o leaderboard.
void RenderLeaderboard(GameState *state, GameTextures *textures, Vector2 mouse);

// RenderZombies renderiza os zumbis no grid.
void RenderZombies(GameState *state, GameTextures *textures);

// RenderHordeStatus renderiza o timer para a próxima horda.
void RenderHordeStatus(GameState *state);
#endif
