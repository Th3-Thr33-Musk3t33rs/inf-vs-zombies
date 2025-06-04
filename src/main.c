#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "game.h"
#include "graphics.h"
#include "raylib.h"
#include "utils.h"

int main(void) {
    // Declaração das variáveis globais de estado, texturas e sons.
    GameState gameState;
    GameTextures gameTextures;
    GameSounds gameSounds;

    // Inicializa o gerador de números aleatórios.
    srand(time(NULL));

    // Inicializa a janela do jogo e define as configurações básicas.
    // também carrega as texturas e estado inicial do jogo.
    InitGame(&gameState, &gameTextures, &gameSounds);

    // Loop principal do jogo.
    while (!WindowShouldClose()) {
        // Pega a posição atual do mouse.
        Vector2 mouse = GetMousePosition();

        UpdateMusicStream(gameSounds.backgroundMusic);

        // Processa a entrada do usuário (cliques, etc.) que afeta o estado do
        // jogo.
        ProcessGameInput(&gameState, mouse);

        BeginDrawing();
        ClearBackground(RAYWHITE); // Limpa a tela a cada frame.

        // Verifica se o jogo não está em estado de Game Over.
        if (!gameState.app.isGameOver) {
            // Se estiver na tela de título, renderiza apenas ela.
            if (gameState.app.onTitleScreen) {
                RenderTitleScreen(BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE);
            } else {
                // Se estiver pausado, renderiza e carrega apenas a lógica do
                // menu de pause.
                if (gameState.app.isPaused) {
                    HandlePause(&gameState, mouse, BASE_WIDTH_INT, BASE_HEIGHT_INT);
                    RenderPause(&gameState, &gameTextures, mouse);
                } else {
                    // Atualiza a lógica do jogo.
                    UpdateGame(&gameState);
                }
                
                PlaySounds(&gameState, &gameSounds);

                // Se o jogo estiver ativo, renderiza todos os elementos do
                // jogo.
                RenderHUD(&gameState, BASE_WIDTH_INT, BASE_HEIGHT_INT,
                          FONT_SIZE, &gameTextures, mouse, &gameSounds);
                RenderCharacterSelector(&gameState, BASE_WIDTH_INT,
                                        BASE_HEIGHT_INT, FONT_SIZE,
                                        &gameTextures, mouse);
                RenderGameGrid(&gameState, BASE_WIDTH_INT, BASE_HEIGHT_INT,
                               &gameTextures, mouse, FONT_SIZE);
                RenderProjectiles(&gameState, BASE_WIDTH_INT, BASE_HEIGHT_INT,
                                  &gameTextures, &gameSounds);
                RenderMoneyBag(&gameState, BASE_WIDTH_INT, BASE_HEIGHT_INT,
                               &gameTextures, mouse, &gameSounds);
                RenderSelectedCharacterPreview(&gameState, &gameTextures,
                                               mouse, BASE_WIDTH_INT,
                                               BASE_HEIGHT_INT);
                RenderPause(&gameState, &gameTextures, &gameSounds, mouse,
                            BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE);
            }
        } else {
            // TODO: Fazer uma Endscreen, com o ranking e opções para jogar denovo ou voltar ao menu inicial.
            // RenderTitleScreen é só um placeholder aqui.
            RenderTitleScreen(BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE);
        }

        EndDrawing();  // Finaliza o desenho.
    }

    // Descarrega as texturas e fecha a janela ao sair do loop.
    UnloadTextures(&gameTextures);
    UnloadSounds(&gameSounds);
    CloseWindow();

    return 0;
}
