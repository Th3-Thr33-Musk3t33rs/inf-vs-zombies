#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include "utils.h"
#include "game.h"
#include "graphics.h"
#include "config.h"


int main(void) {
	// Declaração das variáveis globais de estado e texturas.
	GameState game_state;
	GameTextures game_textures;

    // Inicializa o gerador de números aleatórios.
    srand(time(NULL));

    // Inicializa a janela do jogo e define as configurações básicas.
	// também carrega as texturas e estado inicial do jogo.
    InitGame(&game_state, &game_textures);

    // Loop principal do jogo.
    while (!WindowShouldClose()) {
        // Pega a posição atual do mouse.
        Vector2 mouse = GetMousePosition();

        // Processa a entrada do usuário (cliques, etc.) que afeta o estado do jogo.
        ProcessGameInput(&game_state, mouse, BASE_WIDTH_INT, BASE_HEIGHT_INT);

        BeginDrawing();
        ClearBackground(RAYWHITE); // Limpa a tela a cada frame.

        // Verifica se o jogo não está em estado de Game Over.
        if (!game_state.gameOver) {
            // Se estiver na tela de título, renderiza apenas ela.
            if (game_state.titleScreen) {
                RenderTitleScreen(BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE);
            } else {
				// Atualiza a lógica do jogo.
        		UpdateGame(&game_state);

                // Se o jogo estiver ativo, renderiza todos os elementos do jogo.
                RenderHUD(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE, &game_textures, mouse);
                RenderCharacterSelector(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT, FONT_SIZE, &game_textures, mouse);
                RenderGameGrid(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT, &game_textures, mouse);
                RenderProjectiles(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT, &game_textures);
                RenderMoneyBag(&game_state, BASE_WIDTH_INT, BASE_HEIGHT_INT, &game_textures, mouse);
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
