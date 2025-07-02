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

        if (!gameState.app.onTitleScreen && !gameState.app.isPaused && !gameState.app.onLeaderboard) {
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
            RenderLeaderboard(&gameState, &gameTextures, mousePos);
            gameState.app.onLeaderboard = true;
            if (gameState.app.leavingLeaderboard) {
                InitializeGameState(&gameState, true);
                int hordes[MAX_HORDES] = {0};
                gameState.totalHordes = ReadHordesConfig("config.txt", hordes, MAX_HORDES);
                for (int i = 0; i < gameState.totalHordes; i++) {
                    gameState.hordes[i] = hordes[i];
                }
                LoadLeaderboard(LEADERBOARD_FILE, &gameState);
                gameState.app.leavingLeaderboard = false;
                gameState.app.onLeaderboard = false;
                gameState.app.onTitleScreen = true;
            }
        }
        EndDrawing();
    }

    UnloadTextures(&gameTextures);
    UnloadSounds(&gameSounds);
    CloseWindow();
    return 0;
}
