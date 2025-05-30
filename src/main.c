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
    GameState game_state;
    GameTextures game_textures;
    GameSounds game_sounds;

    // Inicializa o gerador de números aleatórios.
    srand(time(NULL));

    // hordes é a nossa variável com a quantidade de zumbis por horda.
    int hordes[MAX_HORDES] = {0};
    
    // hordes_number é o número de hordas.
    // Em C, quando você passa um array como argumento para uma função,
    // você não está enviando uma cópia do array, você está enviando o endereço de memória do array original.
    // Dessa forma, hordes é passada por referência aqui e será propriamente populada.
    int hordes_number = ReadHordesConfig("config.txt", hordes, MAX_HORDES);

    // Inicializa a janela do jogo e define as configurações básicas.
    // também carrega as texturas e estado inicial do jogo.
    InitGame(&game_state, &game_textures, &game_sounds);

    // Loop principal do jogo.
    while (!WindowShouldClose()) {
        // Pega a posição atual do mouse.
        Vector2 mouse = GetMousePosition();

        // Processa a entrada do usuário (cliques, etc.) que afeta o estado do
        // jogo.
        ProcessGameInput(&game_state, mouse, BASE_WIDTH_INT, BASE_HEIGHT_INT);

        BeginDrawing();
        ClearBackground(RAYWHITE);  // Limpa a tela a cada frame.

        // Verifica se o jogo não está em estado de Game Over.
        if (!game_state.gameOver) {
            // Se estiver na tela de título, renderiza apenas ela.
            if (game_state.titleScreen) {
                RenderTitleScreen(BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE);
            } else {
                // Se estiver pausado, renderiza e carrega apenas a lógica do
                // menu de pause
                if (game_state.pause) {
                    HandlePause(&game_state, mouse, BASE_WIDTH_INT,
                                BASE_HEIGHT_INT);
                    RenderPause(&game_state, &game_textures, &game_sounds,
                                mouse, BASE_WIDTH_INT, BASE_HEIGHT_INT,
                                FONT_SIZE);
                } else {
                    // Atualiza a lógica do jogo.
                    UpdateGame(&game_state);
                }

                // Se o jogo estiver ativo, renderiza todos os elementos do
                // jogo.
                RenderHUD(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT,
                          FONT_SIZE, &game_textures, mouse, &game_sounds);
                RenderCharacterSelector(&game_state, BASE_WIDTH_INT,
                                        BASE_HEIGHT_INT, FONT_SIZE,
                                        &game_textures, mouse);
                RenderGameGrid(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT,
                               &game_textures, mouse, FONT_SIZE);
                RenderProjectiles(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT,
                                  &game_textures, &game_sounds);
                RenderMoneyBag(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT,
                               &game_textures, mouse, &game_sounds);
                RenderSelectedCharacterPreview(&game_state, &game_textures,
                                               mouse, BASE_WIDTH_INT,
                                               BASE_HEIGHT_INT);
                RenderPause(&game_state, &game_textures, &game_sounds, mouse,
                            BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE);
            }
        }
        // TODO: Adicionar tela de Game Over aqui, se game_state.gameOver for
        // true.

        EndDrawing();  // Finaliza o desenho.
    }

    // Descarrega as texturas e fecha a janela ao sair do loop.
    UnloadTextures(&game_textures);
    UnloadSounds(&game_sounds);
    CloseWindow();

    return 0;
}
