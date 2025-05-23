#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h> // Para rand(), srand()
#include <time.h>   // Para time()
#include "utils.h"
#include "game.h"
#include "graphics.h"
#include "config.h"

// Definição das variáveis globais de estado e texturas.
// Elas são declaradas como 'extern' nos seus respectivos headers.
GameState game_state;
GameTextures game_textures;

int main(void) {
    // Inicializa o gerador de números aleatórios.
    srand(time(NULL));

    // Inicializa a janela do jogo e define as configurações básicas.
    InitGame();
    // Carrega as texturas após a inicialização da janela.
    InitializeTextures(&game_textures);
    // Inicializa o estado do jogo.
    InitializeGameState(&game_state);

    // Define o tamanho da fonte base e a escala para a resolução atual.
    int fontSize = (int)(BASE_FONT_SIZE * ((float)BASE_HEIGHT_INT / 720.0f));

    // Loop principal do jogo.
    while (!WindowShouldClose()) {
        // Pega a posição atual do mouse.
        Vector2 mouse = GetMousePosition();

        // Atualiza a lógica do jogo.
        UpdateGame(&game_state);

        // Processa a entrada do usuário (cliques, etc.) que afeta o estado do jogo.
        ProcessGameInput(&game_state, mouse, BASE_WIDTH_INT, BASE_HEIGHT_INT);

        // Início do desenho (renderização).
        BeginDrawing();
        ClearBackground(RAYWHITE); // Limpa a tela a cada frame.

        // Verifica se o jogo não está em estado de Game Over.
        if (!game_state.gameOver) {
            // Se estiver na tela de título, renderiza apenas ela.
            if (game_state.titleScreen) {
                RenderTitleScreen(BASE_WIDTH_INT, BASE_HEIGHT_INT, fontSize);
            } else {
                // Se o jogo estiver ativo, renderiza todos os elementos do jogo.
                RenderHUD(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT, fontSize, &game_textures, mouse);
                RenderCharacterSelector(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT, fontSize, &game_textures, mouse);
                RenderGameGrid(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT, &game_textures, mouse);
                RenderProjectiles(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT, &game_textures);
                RenderPointsBag(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT, &game_textures, mouse);
                RenderSelectedCharacterPreview(&game_state, &game_textures, mouse, BASE_WIDTH_INT, BASE_HEIGHT_INT);
            }
        }
        // TODO: Adicionar tela de Game Over aqui, se game_state.gameOver for true.

        EndDrawing(); // Finaliza o desenho.
    }

    // Descarrega as texturas e fecha a janela ao sair do loop.
    UnloadTextures(&game_textures);
    CloseWindow();

    return 0;
}
