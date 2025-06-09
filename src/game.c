#define _CRT_SECURE_NO_WARNINGS

#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "character_data.h"
#include "config.h"
#include "graphics.h"
#include "utils.h"

// InitGame inicializa o jogo.
void InitGame(GameState *state, GameTextures *textures, GameSounds *sounds) {
    SetTargetFPS(TARGET_FPS);
    InitWindow(BASE_WIDTH_INT, BASE_HEIGHT_INT, GAME_TITLE);
    InitializeTextures(textures);
    InitializeSounds(sounds);
    InitializeGameState(state);
    SetExitKey(0);  // Desabilita a saída com ESC.
}

// Inicializa o estado do jogo com valores padrão.
void InitializeGameState(GameState *state) {
    // Zera toda a estrutura GameState para garantir que não haja lixo de memória.
    *state = (GameState){0};

    // Inicializa valores básicos do estado do jogo(TitleScreen, Money).
    state->app.onTitleScreen = true;
    state->stats.money = INITIAL_MONEY;

    // Seta todas as Tiles como 1 (gramado padrão), exceto as da coluna 0, que possuem valor 0 (botões).
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            if (c == TILE_TYPE_BUTTON) {  // Coluna 0 é para botões/seleção.
                state->tiles[r][c] = TILE_TYPE_BUTTON;
            } else {
                state->tiles[r][c] = TILE_TYPE_GRASS;
            }
        }
    }

    state->moneyBag.shouldRandomizePos = true;

    // hordes é a nossa variável com a quantidade de zumbis por horda.
    int hordes[MAX_HORDES] = {0};

    // hordes_number é o número de hordas.
    // Em C, quando você passa um array como argumento para uma função,
    // você não está enviando uma cópia do array, você está enviando o endereço de memória do array original.
    // Dessa forma, hordes é passada por referência aqui e será propriamente populada.
    int hordes_number = ReadHordesConfig("config.txt", hordes, MAX_HORDES);
}

// Atualiza a lógica principal do jogo por frame.
void UpdateGame(GameState *state, float deltaTime) {
    UpdateCharacters(state, deltaTime);   // Atualiza os estados e animações dos personagens.
    UpdateProjectiles(state, deltaTime);  // Atualiza a lógica dos projéteis.
    UpdateMoneyBag(state, deltaTime);     // Atualiza a lógica da bolsa de dinheiro.

    for (int i = CHAR_TYPE_CHIMPANZINI; i < CHAR_TYPE_COUNT; i++) {
        if (state->characterCooldowns[i] > 0) {
            state->characterCooldowns[i] -= deltaTime;
        } else {
            state->characterCooldowns[i] = 0;
        }
    }
}

// Atualiza os estados e animações dos personagens.
void UpdateCharacters(GameState *state, float deltaTime) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            Character *character = &state->entities.characters[r][c];
            if (!character->exists) continue;  // Se nenhum personagem existe ali, podemos pular.

            character->animationCounter += deltaTime;
            if (character->animationCounter > 0.2f) {  // Muda de frame a cada 0.2s.
                character->currentFrame++;
                character->animationCounter = 0;
            }

            // Lógica de HP.
            if (character->hp <= 0) {
                character->exists = false;
                state->tiles[r][c] = TILE_TYPE_GRASS;  // Tile volta ao estado padrão.
                state->stats.charactersLost++;         // Atualiza stat.
                continue;
            }

            switch (character->type) {
                // Lógica de comportamento do Chimpanzini (geração de dinheiro).
                case CHAR_TYPE_CHIMPANZINI:
                    if (!character->specific.chimpanzini.shining) {
                        if (character->currentFrame > 3) {  // Loop de idle.
                            character->currentFrame = 0;
                            character->specific.chimpanzini.loop++;
                        }

                        if (character->specific.chimpanzini.loop >= 40) {  // Após 40 loops, começa a brilhar.
                            character->specific.chimpanzini.shining = true;
                            character->currentFrame = 4;               // Inicia animação de brilho.
                            character->specific.chimpanzini.loop = 0;  // Reseta loop.
                        }
                    } else {  // Animação enquanto brilha.
                        if (character->currentFrame > 7) {
                            character->currentFrame = 4;  // Mantém no loop do brilho.
                        }
                    }
                    break;

                // Lógica de comportamento do Tralalero (ataque de projétil).
                case CHAR_TYPE_TRALALERO:
                    // Se não estiver no meio da animação de ataque, processa a lógica de idle.
                    if (!character->specific.tralalero.attacking) {
                        if (character->currentFrame > 3) {
                            character->currentFrame = 0;  // Reinicia a animação de idle
                            character->specific.tralalero.loop++;
                        }
                        // Após um número de ciclos de idle, inicia o ataque.
                        if (character->specific.tralalero.loop >= 20) {
                            character->specific.tralalero.attacking = true;
                            character->specific.tralalero.loop = 0;  // Reseta o contador de ciclos
                            character->currentFrame = 4;             // Inicia a animação de ataque (frame 4 a 7)
                        }
                    } else {  // Se estiver na animação de ataque
                        // Quando a animação de ataque terminar...
                        if (character->currentFrame > 7) {
                            // Volta para o estado de idle.
                            character->specific.tralalero.attacking = false;
                            character->currentFrame = 0;

                            // Toca o som de disparo.
                            state->soundToPlay = SOUND_PROJECTILE;
                            state->shouldPlaySound = true;

                            // Procura por um projétil inativo no array para lançar.
                            for (int i = 0; i < MAX_PROJECTILES; i++) {
                                if (!state->entities.projectiles[i].isActive) {
                                    state->entities.projectiles[i].isActive = true;

                                    // Define a posição inicial do projétil, saindo do personagem.
                                    // Os números (+80, +40) são ajustes finos para que o projétil
                                    // saia da "boca" do personagem, e não do canto da tile.
                                    state->entities.projectiles[i].position.x = (GRID_MARGIN_X) + (character->col * 96) + 80;
                                    state->entities.projectiles[i].position.y = (GRID_MARGIN_Y) + (character->row * 78) + 40;

                                    break;
                                }
                            }
                        }
                    }
                    break;

                // Lógica de comportamento do Sahur (idle simples).
                case CHAR_TYPE_SAHUR:
                    if (character->currentFrame == 3) {
                        character->currentFrame = 0;
                    }
                    break;
                // Lógica de comportamento do Lirili (mudança de sprite conforme HP).
                case CHAR_TYPE_LIRILI:
                    if (character->hp < 100 && character->hp >= 75) {
                        character->currentFrame = 1;
                    } else if (character->hp < 75 && character->hp >= 50) {
                        character->currentFrame = 2;
                    } else if (character->hp < 50) {
                        character->currentFrame = 3;
                    } else {
                        character->currentFrame = 0;  // Estado inicial.
                    }
                    break;
                // Lógica de comportamento do Bombardini (carregamento da bomba).
                case CHAR_TYPE_BOMBARDINI:
                    if (!character->specific.bombardini.ready) {
                        character->currentFrame++;
                        if (character->currentFrame == 3) {
                            character->currentFrame = 0;
                            character->specific.bombardini.loop++;
                        }
                        if (character->specific.bombardini.loop >= 300) {  // Após 300 loops, fica "pronto".
                            character->specific.bombardini.ready = true;
                            character->currentFrame = 4;  // Inicia animação de pronto.
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

// Atualiza a lógica dos projéteis.
void UpdateProjectiles(GameState *state, float deltaTime) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            if (state->entities.characters[r][c].specific.tralalero.projecB) {
                // Move o projétil.
                state->entities.characters[r][c].specific.tralalero.projecX += PROJECTILE_SPEED * deltaTime;

                // Se o projétil sair da tela, reseta-o
                if (state->entities.characters[r][c].specific.tralalero.projecX > BASE_WIDTH_INT) {
                    // Posição inicial.
                    state->entities.characters[r][c].specific.tralalero.projecX = (GRID_MARGIN_X + 20) + (c * 96) + 35;

                    // Desativa o projétil.
                    state->entities.characters[r][c].specific.tralalero.projecB = false;
                }
            }
            // Lógica para bombas do Bombardini (se houver)
        }
    }
}

// Atualiza a lógica da bolsa de dinheiro aleatória.
void UpdateMoneyBag(GameState *state, float deltaTime) {
    // Lógica de spawn: Tenta gerar uma nova bolsa se permitido.
    // A lógica só roda se 'shouldRandomizePos' for verdadeiro, evitando gerar uma bolsa em cima da outra.
    if (state->moneyBag.shouldRandomizePos) {
        // Gera um número aleatório para decidir se a bolsa aparece.
        int randomChance = rand() % MONEY_BAG_RANDOMNESS;  // Ajuste este valor para mudar a frequência.

        // --- Debugging ---
        char RandomText[10];
        sprintf(RandomText, "%d", randomChance);
        DrawText(RandomText, 100, 80, FONT_SIZE, BLACK);
        // --- Debugging ---

        if (randomChance == 0 && !state->moneyBag.isActive) {
            state->moneyBag.isActive = true;
            state->moneyBag.shouldRandomizePos = false;                // Impede novo spawn até esta desaparecer.
            state->moneyBag.timeRemainingInFrames = TimeToFrames(30);  // Duração da bolsa (30 segundos).
            state->moneyBag.pulseCounter = 0;
            // Define a posição aleatória da bolsa
            state->moneyBag.position.x = (float)(rand() % (BASE_WIDTH_INT - 150) + 50);    // Evita bordas.
            state->moneyBag.position.x = (float)(rand() % (BASE_HEIGHT_INT - 200) + 150);  // Evita HUD superior
        }
    }

    // Lógica de atualização, somente se a bolsa estiver na tela.
    if (state->moneyBag.isActive) {
        state->moneyBag.timeRemainingInFrames--;  // Decrementa o contador de tempo da bolsa.

        // Se o tempo da bolsa acabar, ela desaparece.
        if (state->moneyBag.timeRemainingInFrames <= 0) {
            state->moneyBag.shouldRandomizePos = true;  // Permite nova randomização.
            state->moneyBag.isActive = false;
            state->stats.moneyBagsMissed++;  // Incrementa o contador de bolsas perdidas.
            return;
        }
    }

    // Lógica de piscar.
    state->moneyBag.pulseCounter++;
    if (state->moneyBag.pulseCounter >= 18) {
        state->moneyBag.isPulsing = !state->moneyBag.isPulsing;  // Alterna o estado de piscar.
        state->moneyBag.pulseCounter = 0;                        // Reseta o contador do pulso.
    }
}

// Processa a entrada do usuário que afeta o estado do jogo (cliques em botões, tiles).
void ProcessGameInput(GameState *state, Vector2 mousePos, GameSounds *sounds) {
    // Lógica da tela de título.
    if (state->app.onTitleScreen) {
        Rectangle playDest = ScaleRectTo720p((int)1280 / 2.5 - 5, (int)720 / 2, 210, BASE_FONT_SIZE, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        if (CheckCollisionPointRec(mousePos, playDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->app.onTitleScreen = false;  // Sai da tela de título.
        }
        return;  // Não processa mais nada se estiver na tela de título e o usuário não começou a jogar.
    }

    // Pausa e despausa o jogo ao apertar ESC ou P
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
        state->app.isPaused = !state->app.isPaused;   // Toggle do pause.
        state->app.characterInHand = CHAR_TYPE_NONE;  // Solta o personagem segurado ao pausar.

        if (state->app.isPaused) {
            PauseMusicStream(sounds->backgroundMusic);
        } else {
            ResumeMusicStream(sounds->backgroundMusic);
        }
    }

    if (state->app.isPaused) {
        HandlePauseMenu(state, mousePos);
        return;  // Não processa mais nada se estiver pausado.
    }

    // Lógica do toggle de modo venda.
    Rectangle sellDest = ScaleRectTo720p(SELL_POS_X - 5, SELL_POS_Y, 110, 50, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    if ((CheckCollisionPointRec(mousePos, sellDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_S)) {
        if (state->app.characterInHand != CHAR_TYPE_SELL_MODE) {
            state->app.characterInHand = CHAR_TYPE_SELL_MODE;
        } else {
            state->app.characterInHand = CHAR_TYPE_NONE;
        }
    }

    // Coleta dinheiro de TODOS os Chimpanzinis brilhantes com a tecla C.
    if (IsKeyPressed(KEY_C)) {
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLUMNS; c++) {
                HandleCharacterInteractions(state, r, c);  // A função checa internamente se é um Chimpanzini brilhante.
            }
        }
    }

    // Lógica do seletor de personagens.
    for (int i = CHAR_TYPE_CHIMPANZINI; i < CHAR_TYPE_COUNT; i++) {
        Rectangle frameDest = {300 + ((i - 1) * 77), 20, 78, 96};
        const CharacterInfo *info = &CHARACTER_INFO[i];

        if (CheckCollisionPointRec(mousePos, frameDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (state->stats.money >= info->cost && state->characterCooldowns[i] <= 0) {
                if (state->app.characterInHand == info->type) {
                    state->app.characterInHand = CHAR_TYPE_NONE;
                } else {
                    state->app.characterInHand = info->type;
                }
                state->soundToPlay = SOUND_SELECT;
                state->shouldPlaySound = true;
            }
        }
    }

    // Lógica de coleta da bolsa de dinheiro.
    if (state->moneyBag.isActive) {
        Rectangle moneyBagDest = ScaleRectTo720p(state->moneyBag.position.x, state->moneyBag.position.y, 78, 96, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        if (CheckCollisionPointRec(mousePos, moneyBagDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->shouldPlaySound = true;
            state->soundToPlay = SOUND_COLLECTBAG;
            state->stats.money += MONEY_BAG_AMOUNT;     // Adiciona dinheiro.
            state->stats.moneyBagsCollected++;          // Incrementa o contador de bolsas coletadas.
            state->moneyBag.isActive = false;           // Remove a bolsa.
            state->moneyBag.shouldRandomizePos = true;  // Permite nova randomização.
        }
    }

    // Lógica de interação com o grid.
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (state->tiles[row][col] == TILE_TYPE_BUTTON) continue;
            Rectangle tileDest = ScaleRectTo720p(GRID_MARGIN_X + (col * 96), GRID_MARGIN_Y + (row * 78), 96, 78, BASE_WIDTH_INT, BASE_HEIGHT_INT);
            if (CheckCollisionPointRec(mousePos, tileDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // Tenta executar uma ação na tile clicada. As funções internas vão cuidar das condições.
                HandleCharacterPlacement(state, row, col);
                HandleCharacterSelling(state, row, col);
                HandleCharacterInteractions(state, row, col);
            }
        }
    }
}

void HandleCharacterPlacement(GameState *state, int row, int col) {
    CharacterType charType = state->app.characterInHand;

    // Se já existe um personagem naquela tile, não podemos sobrescrever outro por cima.
    if (charType == CHAR_TYPE_NONE || state->entities.characters[row][col].exists) {
        return;
    }

    const CharacterInfo *charInfo = &CHARACTER_INFO[charType];

    if (state->stats.money >= charInfo->cost) {
        state->stats.money -= charInfo->cost;
        state->stats.charactersBought++;

        state->characterCooldowns[charType] = charInfo->cooldown;

        // Criação do personagem no grid.
        Character *newChar = &state->entities.characters[row][col];
        *newChar = (Character){0};  // Zera a struct antes de preencher.
        newChar->exists = true;
        newChar->type = charType;
        newChar->hp = charInfo->initialHp;
        newChar->row = row;
        newChar->col = col;

        if (charType == TRALALERO_ID) {
            newChar->specific.tralalero.projecX = (GRID_MARGIN_X + 20) + (col * 96) + 35;
            newChar->specific.tralalero.projecY = GRID_MARGIN_Y + (row * 78);
        }

        state->app.characterInHand = CHAR_TYPE_NONE;
        state->soundToPlay = SOUND_PUT;
        state->shouldPlaySound = true;
    }
}

void HandleCharacterSelling(GameState *state, int row, int col) {
    Character *character = &state->entities.characters[row][col];

    // Só vende se estiver no modo de venda e se houver um personagem na tile.
    if (state->app.characterInHand != CHAR_TYPE_SELL_MODE || !character->exists) {
        return;
    }

    CharacterType charType = character->type;

    const CharacterInfo *charInfo = &CHARACTER_INFO[charType];

    if (charType != CHAR_TYPE_BOMBARDINI) {
        state->stats.money += (float)charInfo->cost / 1.5f;  // Reembolso padrão.
    } else {
        state->stats.money += 10;  // Reembolso do Bombardini, visto que ele é uma bomba que não faz nada até que pisem nele.
    }

    character->exists = false;

    state->stats.charactersSold++;
    state->soundToPlay = SOUND_CANCEL;
    state->shouldPlaySound = true;
}

void HandleCharacterInteractions(GameState *state, int row, int col) {
    Character *character = &state->entities.characters[row][col];
    if (!character->exists) return;

    // Lógica específica para coletar dinheiro do Chimpanzini ao clicar.
    if (character->type == CHAR_TYPE_CHIMPANZINI && character->specific.chimpanzini.shining) {
        character->specific.chimpanzini.shining = false;
        character->specific.chimpanzini.loop = 0;
        character->currentFrame = 0;  // Volta para a animação de idle.
        state->stats.money += 25;
        state->soundToPlay = SOUND_COLLECT;
        state->shouldPlaySound = true;
    }
}

// Lógica dos botões do menu de pause.
void HandlePauseMenu(GameState *state, Vector2 mousePos) {
    Rectangle resumeGlowDest = ScaleRectTo720p(504, (BASE_HEIGHT_INT / 4) + 24, 312, 121 - 48, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle exitGlowDest = ScaleRectTo720p(504, (BASE_HEIGHT_INT / 2) + 24, 312, 121 - 48, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    if (CheckCollisionPointRec(mousePos, resumeGlowDest)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->app.isPaused = false;  // Sai do menu de pause.
        }
    }

    if (CheckCollisionPointRec(mousePos, exitGlowDest)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->app.shouldQuit = true;
        }
    }
}

void UpdateHordeState(GameState *state) {
    HordeState *currentHorde = &state->currentHorde;

    // Todos os zumbis da horda foram mortos?
    if (currentHorde->zombies_alive_in_horde == 0) {
        if (currentHorde->horde_number < state->finalHorde) {
            state->currentHorde.horde_number += 1;
        } else {
            state->app.isGameOver = true;
        }
    }
}
