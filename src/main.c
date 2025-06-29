#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "game.h"
#include "graphics.h"
#include "raylib.h"
#include "utils.h"
#include "types.h"

int main(void) {
    // Declaração das estruturas principais do jogo: estado, texturas e sons.
    GameState gameState;
    GameTextures gameTextures;
    GameSounds gameSounds;

    // Inicializa o gerador de números aleatórios.
    srand(time(NULL));



    // Inicializa a janela do jogo e define as configurações básicas.
    // também carrega as texturas e estado inicial do jogo.
    InitGame(&gameState, &gameTextures, &gameSounds);

    // Chama a função LoadLeaderboard, que pega os valores do arquivo binário e insere no array leaderboard
    LoadLeaderboard(LEADERBOARD_FILE, &gameState);


    // Lê e aplica a configuração de hordas.
    int hordes[MAX_HORDES] = {0};
    gameState.totalHordes = ReadHordesConfig("config.txt", hordes, MAX_HORDES);
    for (int i = 0; i < gameState.totalHordes; i++) {
        gameState.hordes[i] = hordes[i];
    }

    // Loop principal do jogo.
    while (!WindowShouldClose() && !gameState.app.shouldQuit) {
        // 1. ENTRADAS
        Vector2 mousePos = GetMousePosition();

        // Processa a entrada do usuário (cliques, etc.) que afeta o estado do jogo.
        ProcessGameInput(&gameState, mousePos, &gameSounds);

        // 2. ATUALIZAÇÕES
        UpdateMusicStream(gameSounds.backgroundMusic);  // Atualiza o buffer da música a cada frame.

        // A lógica do jogo só é atualizada se não estiver na tela de título ou pausado
        if (!gameState.app.onTitleScreen && !gameState.app.isPaused) {
            UpdateGame(&gameState, GetFrameTime());  // Aqui pegamos o tempo do frame.

            // Toca os sons que foram solicitados durante o ciclo de inputs/updates.
            PlaySounds(&gameState, &gameSounds);
        }

        // 3. RENDERIZAÇÃO
        BeginDrawing();
        ClearBackground(RAYWHITE);  // Limpa a tela a cada frame.

        // Verifica se o jogo não está em estado de Game Over.
        if (!gameState.app.isGameOver) {
            if (gameState.app.onTitleScreen && !gameState.app.viewLeaderboard) {
                RenderTitleScreen(BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE, &gameState, &gameTextures, mousePos);

            } else 
            if (gameState.app.viewLeaderboard) {
                HandleLeaderboardMenu(&gameState, mousePos);
                SaveLeaderboard(LEADERBOARD_FILE, &gameState);
                RenderLeaderboard(&gameState, &gameTextures, mousePos);

            }else
            {
                // Desenha todos os elementos do jogo principal
                RenderGameGrid(&gameState, &gameTextures, mousePos);
                RenderProjectiles(&gameState, &gameTextures);
                RenderBombs(&gameState, &gameTextures);
                RenderZombies(&gameState, &gameTextures);
                RenderHUD(&gameState, &gameTextures, mousePos);
                RenderCharacterSelector(&gameState, &gameTextures, mousePos);
                RenderHordeStatus(&gameState);
                RenderMoneyBag(&gameState, &gameTextures, mousePos);
                RenderSelectedCharacterPreview(&gameState, &gameTextures, mousePos);

                // Se o jogo estiver pausado, desenha o menu de pause POR CIMA de tudo
                if (gameState.app.isPaused) {
                    RenderPause(&gameState, &gameTextures, mousePos);
                }
              
                

            }
        } else {
            // TODO: Fazer uma Endscreen, com o ranking e opções para jogar denovo ou voltar ao menu inicial.
            // RenderTitleScreen é só um placeholder aqui.
            RenderTitleScreen(BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE, &gameState, &gameTextures, mousePos);
        }

        EndDrawing();  // Finaliza o desenho.
    }

    // Descarrega as texturas e fecha a janela ao sair do loop.
    UnloadTextures(&gameTextures);
    UnloadSounds(&gameSounds);
    CloseWindow();

    return 0;
}
