#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "game.h"
#include "graphics.h"
#include "raylib.h"
#include "types.h"
#include "utils.h"

int main(void) {
    GameState gameState;
    GameTextures gameTextures;
    GameSounds gameSounds;
    char name[MAX_INPUT_NAME + 1] = "\0";

    srand(time(NULL));

    InitGame(&gameState, &gameTextures, &gameSounds);

    LoadLeaderboard(LEADERBOARD_FILE, &gameState);

    int hordes[MAX_HORDES] = {0};
    gameState.totalHordes = ReadHordesConfig("config.txt", hordes, MAX_HORDES);
    for (int i = 0; i < gameState.totalHordes; i++) {
        gameState.hordes[i] = hordes[i];
    }

    while (!WindowShouldClose() && !gameState.app.shouldQuit) {
        // 1. ENTRADAS
        Vector2 mousePos = GetMousePosition();

        ProcessGameInput(&gameState, mousePos, &gameSounds);

        // 2. ATUALIZAÇÕES
        UpdateMusicStream(gameSounds.backgroundMusic);

        if (!gameState.app.onTitleScreen && !gameState.app.isPaused) {
            UpdateGame(&gameState, GetFrameTime());
            PlaySounds(&gameState, &gameSounds);
        }

        // 3. RENDERIZAÇÃO
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!gameState.app.isGameOver) {
            if (gameState.app.onTitleScreen) {
                RenderTitleScreen(BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE, &gameState, &gameTextures, mousePos);
            } else if (gameState.app.onLeaderboard) {
                RenderLeaderboard(&gameState, &gameTextures, mousePos);
            } else {
                RenderGameGrid(&gameState, &gameTextures, mousePos);
                RenderProjectiles(&gameState, &gameTextures);
                RenderBombs(&gameState, &gameTextures);
                RenderZombies(&gameState, &gameTextures);
                RenderHUD(&gameState, &gameTextures, mousePos);
                RenderCharacterSelector(&gameState, &gameTextures, mousePos);
                RenderHordeStatus(&gameState);
                RenderMoneyBag(&gameState, &gameTextures, mousePos);
                RenderSelectedCharacterPreview(&gameState, &gameTextures, mousePos);

                if (gameState.app.isPaused) {
                    RenderPause(&gameState, &gameTextures, mousePos);
                }
            }
        } else {
            // TODO: Fazer uma Endscreen, com o ranking e opções para jogar denovo ou voltar ao menu inicial.
            // RenderTitleScreen é só um placeholder aqui.
            RenderGameOverScreen(&gameState, &gameTextures, mousePos);
        }

        EndDrawing();
    }

    UnloadTextures(&gameTextures);
    UnloadSounds(&gameSounds);
    CloseWindow();
    return 0;
}
