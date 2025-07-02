#define _CRT_SECURE_NO_WARNINGS

#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "character_data.h"
#include "config.h"
#include "graphics.h"
#include "types.h"
#include "utils.h"

const Vector2 defaultOffset = {20, -10};
const CharacterInfo CHARACTER_INFO[] = {
    [CHAR_TYPE_CHIMPANZINI] = {CHAR_TYPE_CHIMPANZINI, CHIMPANZINI_COST, CHIMPANZINI_CD, CHIMPAZINI_INITIAL_HP, CHIMPAZINI_TEXTURE, {32, 72, 323, 543}, {323 / 5.0f, 543 / 5.0f}, defaultOffset},
    [CHAR_TYPE_TRALALERO] = {CHAR_TYPE_TRALALERO, TRALALERO_COST, TRALALERO_CD, CHIMPAZINI_INITIAL_HP, TRALALERO_TEXTURE, {13, 57, 186, 144}, {186 / 2.0f, 144 / 2.0f}, {0, 0}},
    [CHAR_TYPE_SAHUR] = {CHAR_TYPE_SAHUR, SAHUR_COST, SAHUR_CD, SAHUR_INITIAL_HP, SAHUR_TEXTURE, {0, 0, 180, 264}, {180 / 2.5f, 244 / 2.5f}, defaultOffset},
    [CHAR_TYPE_LIRILI] = {CHAR_TYPE_LIRILI, LIRILI_COST, LIRILI_CD, LIRILI_INITIAL_HP, LIRILI_TEXTURE, {35, 19, 190, 225}, {190 / 2.5f, 225 / 2.5f}, defaultOffset},
    [CHAR_TYPE_BOMBARDINI] = {CHAR_TYPE_BOMBARDINI, BOMBARDINI_COST, BOMBARDINI_CD, BOMBARDINI_INITIAL_HP, BOMBARDINI_TEXTURE, {200, 205, 620, 610}, {620 / 10.0f, 610 / 10.0f}, {18, 9}},
};

// InitGame inicializa o jogo.
void InitGame(GameState *state, GameTextures *textures, GameSounds *sounds) {
    SetTargetFPS(TARGET_FPS);
    InitWindow(BASE_WIDTH_INT, BASE_HEIGHT_INT, GAME_TITLE);
    InitializeTextures(textures);
    InitializeSounds(sounds);
    InitializeGameState(state, false);
    SetExitKey(0);  // Para não sair do jogo com ESC.
}

// Inicializa o estado do jogo com valores padrão.
void InitializeGameState(GameState *state, bool reset) {
    *state = (GameState){0};
 
    if (reset) {
        // Quando reset = true, o jogo acabou, e deve ir para a leaderboad.
        state->app.onLeaderboard = true;
    } else {
        // Senão, é porque o jogo acabou de ser iniciado.
        state->app.onTitleScreen = true;
    }

    state->stats.money = INITIAL_MONEY;

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            if (c == TILE_TYPE_BUTTON) {
                state->tiles[r][c] = TILE_TYPE_BUTTON;
            } else {
                state->tiles[r][c] = TILE_TYPE_METALLIC;
            }
        }
    }

    state->moneyBag.shouldRandomizePos = true;
    state->horde.state = HORDE_STATE_BETWEEN_WAVES;
    state->horde.spawnTimer = TIME_BETWEEN_HORDES;
    state->horde.currentHorde = 0;
}

// Atualiza os estados e animações dos personagens.
void UpdateCharacters(GameState *state, float deltaTime) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            Character *character = &state->entities.characters[row][col];
            if (!character->exists) continue;

            bool canAnimate = true;

            // Se é um bombardini, e ele está pronto, para de animar.
            if (character->type == CHAR_TYPE_BOMBARDINI && character->specific.bombardini.ready) {
                canAnimate = false;
            }

            // Bonecos trocam de frame a cada 0.2s.
            if (canAnimate) {
                character->animationCounter += deltaTime;
                if (character->animationCounter > FRAME_CHANGE_RATE) {
                    character->currentFrame++;
                    character->animationCounter = 0;
                }
            }
            
            if (character->hp <= 0) {
                character->exists = false;
                state->tiles[row][col] = TILE_TYPE_METALLIC;
                state->stats.charactersLost++;
                continue;
            }

            // Posição da tile atual na tela do jogo (px).
            float generalPosX = GRID_MARGIN_X + (col * TILE_WIDTH);
            float generalPosY = GRID_MARGIN_Y + (row * TILE_HEIGHT);

            switch (character->type) {
                // Lógica de comportamento do Chimpanzini(gerassóis).
                case CHAR_TYPE_CHIMPANZINI:
                    // Frames de idle (0-3).
                    if (!character->specific.chimpanzini.shining) {
                        if (character->currentFrame > 3) {
                            character->currentFrame = 0;
                            character->specific.chimpanzini.loop++;
                        }

                        // A cada 20 loops, começa a brilhar, disponibilizando dinheiro ao clicar.
                        if (character->specific.chimpanzini.loop >= CHIMPANZINI_LOOPS_GENERATE) {
                            character->specific.chimpanzini.shining = true;
                            character->currentFrame = 4;
                            character->specific.chimpanzini.loop = 0;
                        }
                    } else {
                        // Frames enquanto brilha(4-7).
                        if (character->currentFrame > 7) {
                            character->currentFrame = 4;
                        }
                    }
                    break;

                // Lógica de comportamento do Tralalero(peashooter).
                case CHAR_TYPE_TRALALERO:
                    if (!character->specific.tralalero.attacking) {
                        if (character->currentFrame > 3) {
                            character->currentFrame = 0;
                            character->specific.tralalero.loop++;
                        }

                        bool zombieDetectedInRow = false;
                        for (int i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
                            Zombie *zombie = &state->entities.zombies[i];
                            if (zombie->isActive && zombie->row == row) {
                                zombieDetectedInRow = true;
                                break;
                            }
                        }
                        
                        // Dispara a cada 3 loops se há um zumbi na linha.
                        if (character->specific.tralalero.loop >= TRALALERO_PROJECTILE_CD && zombieDetectedInRow) {
                            character->specific.tralalero.attacking = true;
                            character->specific.tralalero.loop = 0;
                            character->currentFrame = 4;
                        }
                    } else {
                        // Animação de ataque.
                        if (character->currentFrame > 7) {
                            character->specific.tralalero.attacking = false;
                            character->currentFrame = 0;

                            state->soundToPlay = SOUND_PROJECTILE;
                            state->shouldPlaySound = true;

                            for (int i = 0; i < MAX_PROJECTILES_ON_SCREEN; i++) {
                                if (!state->entities.projectiles[i].isActive) {
                                    state->entities.projectiles[i].isActive = true;

                                    // (+80, +40) são para o projétil sair do meio da tile.
                                    state->entities.projectiles[i].position.x = generalPosX + 80;
                                    state->entities.projectiles[i].position.y = generalPosY + 40;
                                    break;
                                }
                            }
                        }
                    }
                    break;

                // Lógica de comportamento do Sahur(carnívora).
                case CHAR_TYPE_SAHUR:
                    // Pega os frames (0-1).
                    if (character->currentFrame == 2) {
                        character->currentFrame = 0;
                    }

                    if (character->specific.sahur.cooldown) {
                        if (character->currentFrame > 7) {
                            character->currentFrame = 5;
                            character->specific.sahur.loop++;
                        }
                    }

                    for (int j = 0; j < MAX_ZOMBIES_ON_SCREEN; j++) {
                        Zombie *zombie = &state->entities.zombies[j];

                        if (!character->specific.sahur.cooldown && zombie->isActive) {
                            const CharacterInfo *charInfo = &CHARACTER_INFO[character->type];

                            float posX = generalPosX + charInfo->destOffset.x;
                            float posY = generalPosY + 20 + charInfo->destOffset.y;
                            int zombieGridCol = (int)((zombie->position.x - GRID_MARGIN_X - 20) / TILE_WIDTH);
                            Rectangle recZombie = {zombie->position.x, zombie->position.y, REC_ZOMBIE_WIDTH, REC_ZOMBIE_HEIGHT};
                            Rectangle recSahur = ScaleRectTo720p(posX, posY, charInfo->destSize.x + 20, charInfo->destSize.y - REC_ZOMBIE_WIDTH, BASE_WIDTH_INT, BASE_HEIGHT_INT);

                            if (CheckCollisionRecs(recSahur, recZombie)) {
                                // Se não estiver atacando(0, 1), ela vai para o primeiro frame de ataque (2).
                                if (character->currentFrame < 2) {
                                    character->currentFrame = 2;
                                }

                                // Último frame de ataque.
                                if (character->currentFrame == 4) {
                                    zombie->hp -= ZOMBIE_HP;
                                    state->soundToPlay = SOUND_TUNG;
                                    state->shouldPlaySound = true;
                                    character->specific.sahur.cooldown = true;
                                    if (zombie->hp <= 0) {
                                        zombie->isActive = false;
                                        if (zombie->golden) {
                                            state->stats.money += MONEY_BAG_AMOUNT;
                                        }
                                        state->stats.enemiesKilled++;
                                        state->stats.currentPoints += 100;
                                    }
                                }
                            }
                        }
                    }
                    
                    // Depois de loops descansado, sai do cooldown.
                    if (character->specific.sahur.loop == SAHUR_LOOPS) {
                        character->specific.sahur.cooldown = false;
                        character->currentFrame = 1;
                        character->specific.sahur.loop = 0;
                    }

                    break;
                // Lógica de comportamento do Lirili (mudança de sprite conforme HP) (batata).
                case CHAR_TYPE_LIRILI:
                    if (character->hp < 250 && character->hp >= 150) {
                        character->currentFrame = 1;
                    } else if (character->hp < 150 && character->hp >= 50) {
                        character->currentFrame = 2;
                    } else if (character->hp < 50) {
                        character->currentFrame = 3;
                    } else {
                        character->currentFrame = 0;
                    }
                    break;
                // Lógica de comportamento do Bombardini (carregamento da bomba).
                case CHAR_TYPE_BOMBARDINI:
                    if (!character->specific.bombardini.ready) {
                        if (character->currentFrame > 2) {
                            character->currentFrame = 0;
                            character->specific.bombardini.loop++;
                        }
                        // Após X loops, fica "pronto".
                        if (character->specific.bombardini.loop >= BOMBARDINI_LOOPS && !character->specific.bombardini.ready) {
                            character->specific.bombardini.ready = true;
                            character->currentFrame = 4;
                        }
                    }
                    for (int j = 0; j < MAX_ZOMBIES_ON_SCREEN; j++) {
                        Zombie *zombie = &state->entities.zombies[j];

                        if (character->specific.bombardini.ready && zombie->isActive) {
                            const CharacterInfo *charInfo = &CHARACTER_INFO[character->type];

                            float posX = generalPosX + charInfo->destOffset.x;
                            float posY = generalPosY + charInfo->destOffset.y;
                            int zombieGridCol = (int)((zombie->position.x - GRID_MARGIN_X - 20) / TILE_WIDTH);
                            Rectangle recZombie = {zombie->position.x, zombie->position.y, REC_ZOMBIE_WIDTH, REC_ZOMBIE_HEIGHT};
                            Rectangle recBombardini = ScaleRectTo720p(posX, posY + 10, charInfo->destSize.x / 2.3, charInfo->destSize.y - 20, BASE_WIDTH_INT, BASE_HEIGHT_INT);

                            if (CheckCollisionRecs(recBombardini, recZombie)) {  // Se zumbi pisar no alvo, uma bomba cai do céu.
                                for (int i = 0; i < MAX_PROJECTILES_ON_SCREEN; i++) {
                                    if (!state->entities.bombs[i].isActive) {
                                        state->entities.bombs[i].isActive = true;

                                        // Define a posição inicial da bomba, caindo do céu.
                                        state->entities.bombs[i].position.x = posX;
                                        state->entities.bombs[i].position.y = 0;
                                        state->entities.bombs[i].explosionY = posY + 50;

                                        character->exists = false;
                                        state->tiles[row][col] = TILE_TYPE_METALLIC;
                                    }
                                }
                            }
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
    for (int i = 0; i < MAX_PROJECTILES_ON_SCREEN; i++) {
        Projectile *projectile = &state->entities.projectiles[i];
        if (state->entities.projectiles[i].isActive) {
            state->entities.projectiles[i].position.x += PROJECTILE_SPEED * deltaTime;

            // Se o projétil sair da tela, desativa para poder ser reutilizado.
            if (state->entities.projectiles[i].position.x > BASE_WIDTH_INT) {
                state->entities.projectiles[i].isActive = false;
                continue;
            }

            Rectangle recProjectile = {projectile->position.x, projectile->position.y, REC_PROJ_WIDHT, REC_PROJ_HEIGHT};

            for (int j = 0; j < MAX_ZOMBIES_ON_SCREEN; j++) {
                Zombie *zombie = &state->entities.zombies[j];
                if (zombie->isActive) {
                    Rectangle recZombie = {zombie->position.x, zombie->position.y, REC_ZOMBIE_WIDTH, REC_ZOMBIE_HEIGHT};

                    if (CheckCollisionRecs(recProjectile, recZombie)) {
                        projectile->isActive = false;
                        zombie->hp -= PROJECTILE_DAMAGE;
                        state->soundToPlay = SOUND_HIT;
                        state->shouldPlaySound = true;

                        if (zombie->hp <= 0) {
                            zombie->isActive = false;
                            if (zombie->golden) {
                                state->stats.money += MONEY_BAG_AMOUNT;
                            }
                            state->stats.enemiesKilled++;
                            state->stats.currentPoints += ZOMBIE_KILL_SCORE;
                        }
                        break;
                    }
                }
            }
        }
    }
}

// Atualiza a lógica das bombas.
void UpdateBombs(GameState *state, float deltaTime) {
    for (int i = 0; i < MAX_PROJECTILES_ON_SCREEN; i++) {
        Bomb *bomb = &state->entities.bombs[i];
        if (state->entities.bombs[i].isActive) {
            state->entities.bombs[i].position.y += BOMB_SPEED * deltaTime;

            Rectangle recBomb = {bomb->position.x, bomb->position.y, 40, 80};

            for (int j = 0; j < MAX_ZOMBIES_ON_SCREEN; j++) {
                Zombie *zombie = &state->entities.zombies[j];
                if (zombie->isActive) {
                    Rectangle recExplosion = {state->entities.bombs[i].position.x, state->entities.bombs[i].explosionY, REC_ZOMBIE_WIDTH, REC_ZOMBIE_HEIGHT};
                    Rectangle recZombie = {zombie->position.x, zombie->position.y, REC_ZOMBIE_WIDTH, REC_ZOMBIE_HEIGHT};

                    if (CheckCollisionRecs(recBomb, recExplosion)) {
                        bomb->isActive = false;
                        if (CheckCollisionRecs(recExplosion, recZombie)) {
                            zombie->hp -= ZOMBIE_HP;
                            state->soundToPlay = SOUND_EXPLOSION;
                            state->shouldPlaySound = true;

                            if (zombie->hp <= 0) {
                                zombie->isActive = false;
                                if (zombie->golden) {
                                    state->stats.money += MONEY_BAG_AMOUNT;
                                }
                                state->stats.enemiesKilled++;
                                state->stats.currentPoints += 100;
                            }
                        }
                    }
                }
            }
        }
    }
}

// Atualiza a lógica da bolsa de dinheiro aleatória.
void UpdateMoneyBag(GameState *state, float deltaTime) {
    if (state->moneyBag.shouldRandomizePos) {
        int randomChance = rand() % MONEY_BAG_RANDOMNESS;  // Ajustar esse valor muda a frequência da moneybag.

        if (randomChance == 0 && !state->moneyBag.isActive) {
            state->moneyBag.isActive = true;
            state->moneyBag.shouldRandomizePos = false;                // Impede novo spawn até esta desaparecer.
            state->moneyBag.timeRemainingInFrames = TimeToFrames(30);  // Duração da bolsa (30 segundos).
            state->moneyBag.pulseCounter = 0;
            state->moneyBag.position.x = (float)(rand() % BASE_WIDTH_INT - 100);  // Evita bordas.
            state->moneyBag.position.x = (float)(rand() % BASE_HEIGHT_INT - 50);  // Evita HUD superior.
        }
    }

    if (state->moneyBag.isActive) {
        state->moneyBag.timeRemainingInFrames--;

        if (state->moneyBag.timeRemainingInFrames <= 0) {
            state->moneyBag.shouldRandomizePos = true;
            state->moneyBag.isActive = false;
            state->stats.moneyBagsMissed++;
            return;
        }
    }

    // Lógica de piscar.
    state->moneyBag.pulseCounter++;
    if (state->moneyBag.pulseCounter >= 18) {
        state->moneyBag.isPulsing = !state->moneyBag.isPulsing;  // Alterna o estado de piscar.
        state->moneyBag.pulseCounter = 0;
    }
}

// Processa a entrada do usuário que afeta o estado do jogo (cliques em botões, tiles).
void ProcessGameInput(GameState *state, Vector2 mousePos, GameSounds *sounds) {
    if (state->app.onTitleScreen) {
        int glowYOffset = 24;
        Rectangle playGlowDest = ScaleRectTo720p(BUTTONS_X, (BASE_HEIGHT_FLOAT / 2.5f) + glowYOffset, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        if (CheckCollisionPointRec(mousePos, playGlowDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->app.onTitleScreen = false;  // Sai da tela de título.
        }

        Rectangle leaderboardGlowDest = ScaleRectTo720p(BUTTONS_X, BASE_HEIGHT_FLOAT / 1.7f + glowYOffset, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        if (CheckCollisionPointRec(mousePos, leaderboardGlowDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->app.onLeaderboard = true;
            state->app.onTitleScreen = false;
        }

        Rectangle exitGlowDest = ScaleRectTo720p(BUTTONS_X, (BASE_HEIGHT_FLOAT / 1.3f) + glowYOffset, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        if (CheckCollisionPointRec(mousePos, exitGlowDest)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                state->app.shouldQuit = true;
            }
        }
        return;  // Não processa mais nada se estiver na tela de título e o usuário não começou a jogar.
    }

    if (state->app.onLeaderboard) {
        HandleLeaderboardMenu(state, mousePos);
        return;  // Não processa mais nada se estiver na leaderboard.
    }

    // Pausa e despausa o jogo ao apertar ESC ou P
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
        state->app.isPaused = !state->app.isPaused;
        state->app.characterInHand = CHAR_TYPE_NONE;  // Solta o personagem segurado ao pausar.

        if (state->app.isPaused) {
            PauseMusicStream(sounds->backgroundMusic);
        } else {
            ResumeMusicStream(sounds->backgroundMusic);
        }
    }

    if (state->app.isPaused) {
        HandlePauseMenu(state, mousePos, sounds);
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
                HandleCharacterInteractions(state, r, c);
            }
        }
    }

    // Lógica do seletor de personagens.
    for (int f = CHAR_TYPE_CHIMPANZINI; f < CHAR_TYPE_COUNT; f++) {
        const CharacterInfo *info = &CHARACTER_INFO[f];
        int charIndex = f - 1;
        Rectangle frameDest = ScaleRectTo720p(300 + (charIndex * TILE_HEIGHT), 20, TILE_WIDTH, TILE_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        bool canPick = state->stats.money >= info->cost && state->characterCooldowns[f] <= 0;
        if (((CheckCollisionPointRec(mousePos, frameDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_ONE + (charIndex))) && canPick) {
            state->shouldPlaySound = true;
            state->soundToPlay = SOUND_SELECT;
            if (state->app.characterInHand != info->type) {
                state->app.characterInHand = info->type;
            } else {
                state->app.characterInHand = CHAR_TYPE_NONE;
            }
            state->soundToPlay = SOUND_SELECT;
            state->shouldPlaySound = true;
        }
    }

    // Lógica de coleta da bolsa de dinheiro.
    if (state->moneyBag.isActive) {
        Rectangle moneyBagDest = ScaleRectTo720p(state->moneyBag.position.x, state->moneyBag.position.y, TILE_WIDTH, TILE_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        if (CheckCollisionPointRec(mousePos, moneyBagDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->shouldPlaySound = true;
            state->soundToPlay = SOUND_COLLECTBAG;
            state->stats.money += MONEY_BAG_AMOUNT;
            state->stats.moneyBagsCollected++;
            state->moneyBag.isActive = false;
            state->moneyBag.shouldRandomizePos = true;
        }
    }

    // Lógica de interação com o grid.
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            if (state->tiles[row][col] == TILE_TYPE_BUTTON) continue;
            Rectangle tileDest = ScaleRectTo720p(GRID_MARGIN_X + (col * TILE_WIDTH), GRID_MARGIN_Y + (row * TILE_HEIGHT), TILE_WIDTH, TILE_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);
            if (CheckCollisionPointRec(mousePos, tileDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (state->app.characterInHand == CHAR_TYPE_SELL_MODE) {
                    HandleCharacterSelling(state, row, col);
                } else if (state->app.characterInHand > CHAR_TYPE_NONE && state->app.characterInHand < CHAR_TYPE_COUNT) {
                    HandleCharacterPlacement(state, row, col);
                } else {
                    HandleCharacterInteractions(state, row, col);
                }
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
        newChar->exists = true;
        newChar->type = charType;
        newChar->hp = charInfo->initialHp;
        newChar->row = row;
        newChar->col = col;

        switch (charType) {
            case CHAR_TYPE_TRALALERO:
                newChar->specific.tralalero.projecX = GRID_MARGIN_X + (col * TILE_WIDTH) + 55;
                newChar->specific.tralalero.projecY = GRID_MARGIN_Y + (row * TILE_HEIGHT);
                break;
            case CHAR_TYPE_SAHUR:
                newChar->specific.sahur.cooldown = false;
                newChar->currentFrame = 0;
                newChar->specific.sahur.loop = 0;
                break;
            default:
                break;
        }

        state->app.characterInHand = CHAR_TYPE_NONE;
        state->soundToPlay = SOUND_PUT;
        state->shouldPlaySound = true;
    }
}

void HandleCharacterSelling(GameState *state, int row, int col) {
    Character *character = &state->entities.characters[row][col];

    // Só vende se houver um personagem na tile.
    if (!character->exists) {
        return;
    }

    CharacterType charType = character->type;

    const CharacterInfo *charInfo = &CHARACTER_INFO[charType];

    if (charType != CHAR_TYPE_BOMBARDINI) {
        state->stats.money += (float)charInfo->cost * DEFAULT_REIMBURSEMENT_RATE;  // Reembolso padrão.
    } else {
        state->stats.money += BOMBARDINI_REIMBURSEMENT;  // Reembolso do Bombardini, visto que ele é uma bomba que não faz nada até que pisem nele.
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
        character->currentFrame = 0;
        state->stats.money += CHIMPAZINI_MONEY_AWARD;
        state->soundToPlay = SOUND_COLLECT;
        state->shouldPlaySound = true;
    }
}

// Lógica dos botões do menu de pause.
void HandlePauseMenu(GameState *state, Vector2 mousePos, GameSounds *sounds) {
    Rectangle resumeGlowDest = ScaleRectTo720p(BUTTONS_X, (BASE_HEIGHT_INT / 4) + BUTTONS_TILE_HEIGHT, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle exitGlowDest = ScaleRectTo720p(BUTTONS_X, (BASE_HEIGHT_INT / 2) + BUTTONS_TILE_HEIGHT, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    if (CheckCollisionPointRec(mousePos, resumeGlowDest)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->app.isPaused = false;
            ResumeMusicStream(sounds->backgroundMusic);
        }
    }

    if (CheckCollisionPointRec(mousePos, exitGlowDest)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->app.shouldQuit = true;
        }
    }
}

// Lógica dos botões do menu de leaderboard.
void HandleLeaderboardMenu(GameState *state, Vector2 mousePos) {
    state->app.onLeaderboard = true;

    state->app.leavingLeaderboard = false;
    Rectangle backGlowDest = ScaleRectTo720p(BUTTONS_X, (BASE_HEIGHT_FLOAT / 1.3f) + 24, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle saveGlowDest = ScaleRectTo720p(BUTTONS_X, (BASE_HEIGHT_FLOAT / 1.7f) + 24, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    if (CheckCollisionPointRec(mousePos, backGlowDest)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state->app.onTitleScreen = true;
            state->app.onLeaderboard = false;
            state->app.leavingLeaderboard = true;
        }
    }

    if (state->app.isGameOver && CheckCollisionPointRec(mousePos, saveGlowDest) && state->insertedCharsOnLeaderboard == 3) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            SaveLeaderboard(LEADERBOARD_FILE, state);
            state->app.onTitleScreen = true;
            state->app.onLeaderboard = false;
            state->app.leavingLeaderboard = true;
            LoadLeaderboard(LEADERBOARD_FILE, state);
        }
    }
}

void SpawnZombie(GameState *state) {
    for (int i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
        Zombie *zombie = &state->entities.zombies[i];

        if (zombie->isActive) continue;

        zombie->isActive = true;
        zombie->hp = ZOMBIE_HP;
        zombie->state = ZOMBIE_WALKING;
        zombie->row = rand() % ROWS;  // Sorteia uma linha de 0 a 6 para spawnar o zumbi.
        int goldenZombie = rand() % GOLDEN_ZOMBIE_CHANCE;
        if (goldenZombie == 0) {
            zombie->golden = true;
        }

        zombie->position.x = BASE_WIDTH_INT;
        zombie->position.y = GRID_MARGIN_Y + (TILE_HEIGHT * (zombie->row + (1 / 2.0f))) - (TILE_WIDTH / 2.0f);

        return;
    }
}

int CountActiveZombies(GameState *state) {
    int count = 0;
    for (int i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
        if (state->entities.zombies[i].isActive) {
            count++;
        }
    }
    return count;
}

void UpdateHordeLogic(GameState *state, float deltaTime) {
    Horde *horde = &state->horde;

    switch (horde->state) {
        case HORDE_STATE_BETWEEN_WAVES:
            horde->spawnTimer -= deltaTime;
            if (horde->spawnTimer <= 0) {
                horde->zombiesToSpawnInHorde = state->hordes[horde->currentHorde];
                horde->state = HORDE_STATE_SPAWNING;
                state->stats.currentWave = horde->currentHorde + 1;
                horde->spawnTimer = 1.0f;
            }
            break;

        case HORDE_STATE_SPAWNING:
            if (horde->zombiesToSpawnInHorde > 0) {
                horde->spawnTimer -= deltaTime;
                if (horde->spawnTimer <= 0) {
                    SpawnZombie(state);
                    horde->zombiesToSpawnInHorde--;
                    horde->spawnTimer = (float)(rand() % 3 + 3);  // Nasce um zumbi a cada 2-4 segundos.
                }
            } else {
                // Se todos os zumbis da horda foram criados, espera o jogador limpá-los.
                horde->state = HORDE_STATE_WAITING_CLEAR;
            }
            break;
        case HORDE_STATE_WAITING_CLEAR:
            if (CountActiveZombies(state) == 0) {
                horde->currentHorde++;

                if (horde->currentHorde >= state->totalHordes) {
                    state->app.isGameOver = true;
                    horde->state = HORDE_STATE_INACTIVE;
                } else {
                    horde->state = HORDE_STATE_BETWEEN_WAVES;
                    horde->spawnTimer = TIME_BETWEEN_HORDES;
                }
            }
            break;
        case HORDE_STATE_INACTIVE:
            break;
        default:
            break;
    }
}

// UpdateZombies atualiza os zumbis e suas animações.
void UpdateZombies(GameState *state, float deltaTime) {
    for (int i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
        Zombie *zombie = &state->entities.zombies[i];
        if (!zombie->isActive) continue;

        // Calcula em qual coluna do grid a "boca" do zumbi está.
        int zombieGridCol = (int)((zombie->position.x - GRID_MARGIN_X - 20) / TILE_WIDTH);
        if (zombieGridCol < 0) zombieGridCol = 0;
        if (zombieGridCol >= COLUMNS) zombieGridCol = COLUMNS - 1;

        Character *character = &state->entities.characters[zombie->row][zombieGridCol];
        bool isCollidingWithCharacter = (character->exists && zombie->position.x < (GRID_MARGIN_X + (zombieGridCol * TILE_WIDTH) + 70));

        // Se há uma character na frente do zumbi, ele para para comer.
        if (zombie->state == ZOMBIE_WALKING && isCollidingWithCharacter) {
            if (!(character->type == CHAR_TYPE_BOMBARDINI && character->specific.bombardini.ready)) {
                zombie->state = ZOMBIE_EATING;
                zombie->damageTimer = 0;
            }
        } else if (zombie->state == ZOMBIE_EATING && !character->exists) {
            zombie->state = ZOMBIE_WALKING;
        }

        switch (zombie->state) {
            case ZOMBIE_WALKING:
                zombie->animationCounter += deltaTime;
                if (zombie->animationCounter > 0.25f) {
                    // Pega um dos 6 frames da sprite do zumbi na ordem.
                    zombie->currentFrame = (zombie->currentFrame + 1) % 6;
                    zombie->animationCounter = 0;
                }
                zombie->position.x -= ZOMBIE_SPEED * deltaTime;
                break;
            case ZOMBIE_EATING:
                zombie->animationCounter += deltaTime;
                if (zombie->animationCounter > 0.1f) {
                    zombie->currentFrame = (zombie->currentFrame + 1) % 6;
                    zombie->animationCounter = 0;
                }

                zombie->damageTimer += deltaTime;
                if (zombie->damageTimer > ZOMBIE_DMG_CD_SECONDS) {
                    character->hp -= ZOMBIE_DAMAGE;
                    zombie->damageTimer = 0;
                }

                if (zombie->eatSoundTimer == 0.0f) {
                    state->soundToPlay = SOUND_EAT;
                    state->shouldPlaySound = true;
                }

                zombie->eatSoundTimer += deltaTime;
                if (zombie->eatSoundTimer > 2.0f) {
                    zombie->eatSoundTimer = 0;
                }

                if (!character->exists) {
                    zombie->state = ZOMBIE_WALKING;
                }
                break;
            default:
                break;
        }

        if (zombie->position.x < GRID_MARGIN_X) {
            state->app.isGameOver = true;
            state->soundToPlay = SOUND_END_GAME;
            state->shouldPlaySound = true;
        }
    }
}

// UpdateCooldowns atualiza os cooldowns para todos os personagens do seletor.
void UpdateCooldowns(GameState *state, float deltaTime) {
    for (int i = CHAR_TYPE_CHIMPANZINI; i < CHAR_TYPE_COUNT; i++) {
        if (state->characterCooldowns[i] > 0) {
            state->characterCooldowns[i] -= deltaTime;
        } else {
            state->characterCooldowns[i] = 0;
        }
    }
}

// Atualiza a lógica principal do jogo por frame.
void UpdateGame(GameState *state, float deltaTime) {
    UpdateHordeLogic(state, deltaTime);
    UpdateZombies(state, deltaTime);
    UpdateCharacters(state, deltaTime);
    UpdateProjectiles(state, deltaTime);
    UpdateBombs(state, deltaTime);
    UpdateMoneyBag(state, deltaTime);
    UpdateCooldowns(state, deltaTime);
}
