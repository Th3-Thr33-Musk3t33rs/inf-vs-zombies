#define _CRT_SECURE_NO_WARNINGS

#include "graphics.h"

#include <math.h>
#include <stdio.h>

#include "character_data.h"
#include "config.h"
#include "raylib.h"
#include "utils.h"

// Carrega todas as texturas necessárias para o jogo.
void InitializeTextures(GameTextures *textures) {
    // Carrega texturas básicas do ambiente e elementos do jogo.
    textures->metallicTile = LoadTexture("assets/elements/metallictile.png");
    textures->buttonTile = LoadTexture("assets/elements/buttontilemetallic.png");
    textures->statsFrame = LoadTexture("assets/elements/statsframe.png");
    textures->optionFrame = LoadTexture("assets/elements/option.png");
    textures->frame = LoadTexture("assets/elements/frame2.png");
    textures->moneyIcon = LoadTexture("assets/elements/money.png");
    textures->projectile = LoadTexture("assets/elements/projectile.png");
    textures->bomb = LoadTexture("assets/elements/bomb.png");
    textures->zombie = LoadTexture("assets/characters/zombie.png");

    // Carrega todas as texturas de animação dos personagens automaticamente.
    char path[100];
    for (int i = CHAR_TYPE_CHIMPANZINI; i < CHAR_TYPE_COUNT; i++) {  // Itera sobre os tipos de personagem
        for (int t = 0; t < 8; t++) {                                // Itera sobre os frames de animação
            sprintf(path, "assets/characters/%s%d.png", CHARACTER_INFO[i].textureName, t);
            textures->characterTextures[i][t] = LoadTexture(path);
        }
        // Carrega as texturas dos ícones dos personagens para o seletor.
        sprintf(path, "assets/characters/%sframe.png", CHARACTER_INFO[i].textureName);
        textures->characterFrames[i] = LoadTexture(path);
    }
}

// Descarrega todas as texturas da memória.
void UnloadTextures(GameTextures *textures) {
    UnloadTexture(textures->metallicTile);
    UnloadTexture(textures->buttonTile);
    UnloadTexture(textures->statsFrame);
    UnloadTexture(textures->optionFrame);
    UnloadTexture(textures->frame);
    UnloadTexture(textures->moneyIcon);
    UnloadTexture(textures->projectile);
    UnloadTexture(textures->bomb);

    for (int i = 1; i < CHAR_TYPE_COUNT; i++) {
        for (int t = 0; t < 8; t++) {
            // Descarrega as texturas de animação dos personagens.
            UnloadTexture(textures->characterTextures[i][t]);
        }
        // Descarrega as texturas dos ícones dos personagens.
        UnloadTexture(textures->characterFrames[i]);
    }
}

// Carrega os sons do jogo.
void InitializeSounds(GameSounds *sounds) {
    InitAudioDevice();
    sounds->backgroundMusic = LoadMusicStream("assets/sfx/bgmusic.wav");
    PlayMusicStream(sounds->backgroundMusic);  // Inicia a música de fundo.

    sounds->cancelSFX = LoadSound("assets/sfx/cancel.wav");
    sounds->selectSFX = LoadSound("assets/sfx/select.wav");
    sounds->collectSFX = LoadSound("assets/sfx/collect.wav");
    sounds->collectBagSFX = LoadSound("assets/sfx/collectbag.mp3");
    sounds->putSFX = LoadSound("assets/sfx/put.wav");
    sounds->projectileSFX = LoadSound("assets/sfx/projectile.wav");
    sounds->explosionSFX = LoadSound("assets/sfx/explosion.wav");
    sounds->hitSFX = LoadSound("assets/sfx/hit.wav");
    sounds->eatSFX = LoadSound("assets/sfx/eating.wav");
    sounds->endGameSFX = LoadSound("assets/sfx/end.wav");
}

// Descarrega os sons do jogo.
void UnloadSounds(GameSounds *sounds) {
    UnloadSound(sounds->cancelSFX);
    UnloadSound(sounds->selectSFX);
    UnloadSound(sounds->collectSFX);
    UnloadSound(sounds->collectBagSFX);
    UnloadSound(sounds->hitSFX);
    UnloadSound(sounds->putSFX);
    UnloadSound(sounds->projectileSFX);
    UnloadMusicStream(sounds->backgroundMusic);
    CloseAudioDevice();
}

// Toca os sons do jogo.
void PlaySounds(GameState *state, GameSounds *sounds) {
    if (!state->shouldPlaySound) return;

    switch (state->soundToPlay) {
        case SOUND_PROJECTILE:
            PlaySound(sounds->projectileSFX);
            break;
        case SOUND_EXPLOSION:
            PlaySound(sounds->explosionSFX);
            break;
        case SOUND_SELECT:
            PlaySound(sounds->selectSFX);
            break;
        case SOUND_COLLECT:
            PlaySound(sounds->collectSFX);
            break;
        case SOUND_COLLECTBAG:
            PlaySound(sounds->collectBagSFX);
            break;
        case SOUND_CANCEL:
            PlaySound(sounds->cancelSFX);
            break;
        case SOUND_PUT:
            PlaySound(sounds->putSFX);
            break;
        case SOUND_HIT:
            PlaySound(sounds->hitSFX);
            break;
        case SOUND_EAT:
            PlaySound(sounds->eatSFX);
            break;
        case SOUND_END_GAME:
            PlaySound(sounds->endGameSFX);
            break;
        default:
            break;
    }
    state->shouldPlaySound = false;
    state->soundToPlay = 0;
}

// Renderiza a tela de título do jogo.
void RenderTitleScreen(int screenWidth, int screenHeight, int fontSize) {
    DrawText(GAME_TITLE, screenWidth / 3, screenHeight / 3, fontSize, BLACK);
    DrawText("Play Game", screenWidth / 2.5, screenHeight / 2, fontSize, BLACK);

    // Posição e tamanho do botão "Play Game" para detecção de colisão.
    Rectangle playDest = ScaleRectTo720p((int)1280 / 2.5 - 5, (int)720 / 2, 210, fontSize, screenWidth, screenHeight);

    // Highlight visual do botão "Play Game" ao passar o mouse.
    if (CheckCollisionPointRec(GetMousePosition(), playDest)) {
        DrawRectangleRec(playDest, ColorAlpha(YELLOW, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

// Renderiza o HUD com dinheiro e botão de venda.
void RenderHUD(GameState *state, GameTextures *textures, Vector2 mouse) {
    char moneyText[10];
    sprintf(moneyText, "%d", state->stats.money);  // Converte o money para string.

    // Posição do texto de dinheiro.
    Vector2 textmoney = ScaleTo720p(110, 50, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    int textWidth = MeasureText(moneyText, FONT_SIZE);

    // Retângulo de destino para o ícone de moeda.
    Rectangle moneyCountDest = {
        textmoney.x + textWidth + 10,
        textmoney.y + (FONT_SIZE / 2.1f) - (35 * BASE_HEIGHT_INT / 720.0f) / 2.0f, 35, 35};
    Rectangle moneyCountSource = {275, 26, 179, 179};  // Região da spritesheet do ícone.

    // Renderiza o texto do número de dinheiro.
    DrawText(moneyText, (int)textmoney.x, (int)textmoney.y, FONT_SIZE, BLACK);

    // Renderiza o ícone de moeda.
    DrawTexturePro(textures->moneyIcon, moneyCountSource, moneyCountDest, (Vector2){0, 0}, 0.0f, WHITE);

    // Posição e tamanho do botão "SELL".
    // Usamos as constantes de config.h para a posição.
    Rectangle sellDest = ScaleRectTo720p(SELL_POS_X - 5, SELL_POS_Y, 110, 50, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    DrawText("SELL", SELL_POS_X, SELL_POS_Y, FONT_SIZE, BLACK);

    // Highlight visual do botão "SELL" ao passar o mouse.
    if (CheckCollisionPointRec(mouse, sellDest) && !state->app.isPaused) {
        DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }

    // Highlight se o modo de venda estiver ativo.
    if (state->app.characterInHand == CHAR_TYPE_SELL_MODE) {
        DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
    }
}

// Renderiza o seletor de personagens na parte superior da tela.
void RenderCharacterSelector(GameState *state, GameTextures *textures, Vector2 mouse) {
    char costText[10];  // Para exibir o custo do personagem.

    for (int f = CHAR_TYPE_CHIMPANZINI; f < CHAR_TYPE_COUNT; f++) {  // Itera sobre os 5 slots de personagem no seletor.
        const CharacterInfo *charInfo = &CHARACTER_INFO[f];

        // Renderiza quadro base.
        Rectangle frameDest = ScaleRectTo720p(300 + ((f - 1) * 77), 20, 78, 96, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        Rectangle frameSource = {0, 0, textures->frame.width, textures->frame.height};
        DrawTexturePro(textures->frame, frameSource, frameDest, (Vector2){0, 0}, 0.0f, WHITE);

        // Renderiza ícone do personagem.
        // A fonte da textura do frame do personagem é a mesma para todos os ícones no seletor,
        // então podemos usar um índice fixo como 2 (Sahur) para pegar as dimensões.
        Rectangle charFrameDest = ScaleRectTo720p(300 + ((f - 1) * 77), 29, 78, 82.75f, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        Rectangle charFrameSource = {0, 0, textures->characterFrames[2].width, textures->characterFrames[2].height / 1.5f};
        DrawTexturePro(textures->characterFrames[f], charFrameSource, charFrameDest, (Vector2){0, 0}, 0.0f, WHITE);

        // Renderiza o custo do personagem embaixo do quadro.
        sprintf(costText, "%d", charInfo->cost);
        Vector2 costPos = ScaleTo720p(310 + ((f - 1) * 77), 117, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        DrawText(costText, 12 + (int)costPos.x, (int)costPos.y, FONT_SIZE / 1.5, BLACK);

        if (state->stats.money < charInfo->cost || state->characterCooldowns[f] > 0) {
            // Cinza, pois não consegue comprar por falta de dinheiro ou personagem está em cooldown.
            DrawRectangleRec(frameDest, ColorAlpha(DARKGRAY, 0.6f));
        } else if (CheckCollisionPointRec(mouse, frameDest) && !state->app.isPaused) {
            // Highlight visual do personagem selecionado ao passar o mouse.
            DrawRectangleRec(frameDest, ColorAlpha(YELLOW, 0.3f));
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }

        // Highlight se este personagem estiver atualmente selecionado.
        if (state->app.characterInHand == charInfo->type) {
            DrawRectangleRec(frameDest, ColorAlpha(BLUE, 0.2f));  // Um highlight diferente para o selecionado
        }

        // Highlight que desaparece de acordo com cooldown do personagem.
        if (state->characterCooldowns[f] > 0) {
            float cooldownFraction = state->characterCooldowns[f] / charInfo->cooldown;
            float overlayHeight = frameDest.height * cooldownFraction;
            Rectangle cooldownOverlayRect = {frameDest.x, frameDest.y, frameDest.width, overlayHeight};

            DrawRectangleRec(cooldownOverlayRect, ColorAlpha(BLACK, 0.7f));
        }
    }
}

// Renderiza o painel de estatísticas.
void RenderStatsPanel(PlayerStats *stats, GameTextures *textures) {
    Vector2 Origin = {0, 0};

    // Posicionamento das estatísticas gerais, ondas e pontos.
    Vector2 textStatsPos = ScaleTo720p(70, 280, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Vector2 textWavePos = ScaleTo720p(135, 220, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Vector2 textPointsPos = ScaleTo720p(70, 645, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    // Renderiza o quadro de estatísticas, e os textos dentro dele.
    // Rectangle statsDest = ScaleRectTo720p(0, GRID_MARGIN_Y, BASE_WIDTH_INT - ((COLUMNS + 1) * 96) - (BASE_WIDTH_INT - (GRID_MARGIN_X + (COLUMNS + 1) * 96)), ROWS * 78, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle statsFrameDest = ScaleRectTo720p(0, GRID_MARGIN_Y, GRID_MARGIN_X - 10, ROWS * 78, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle statsSource = {0, 0, textures->statsFrame.width, textures->statsFrame.height};
    DrawTexturePro(textures->statsFrame, statsSource, statsFrameDest, Origin, 0.0f, WHITE);

    char buffer[64];

    // Renderização de textos das estatísticas.
    sprintf(buffer, "WAVE: %d", stats->currentWave);
    DrawText(buffer, (int)textWavePos.x, (int)textWavePos.y, FONT_SIZE, RED);

    sprintf(buffer, "Characters Bought: %d", stats->charactersBought);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y, FONT_SIZE / 1.5, RED);
    sprintf(buffer, "Characters Sold: %d", stats->charactersSold);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y + 30, FONT_SIZE / 1.5, RED);
    sprintf(buffer, "Characters Lost: %d", stats->charactersLost);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y + 60, FONT_SIZE / 1.5, RED);
    sprintf(buffer, "Enemies Killed: %d", stats->enemiesKilled);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y + 90, FONT_SIZE / 1.5, RED);
    sprintf(buffer, "Bags Collected: %d", stats->moneyBagsCollected);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y + 120, FONT_SIZE / 1.5, RED);
    sprintf(buffer, "Bags Missed: %d", stats->moneyBagsMissed);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y + 150, FONT_SIZE / 1.5, RED);

    sprintf(buffer, "Points: %d", stats->currentPoints);
    DrawText(buffer, (int)textPointsPos.x, (int)textPointsPos.y, FONT_SIZE / 2, RED);
}

// Renderiza o grid principal do jogo, incluindo as tiles e personagens.
void RenderGameGrid(GameState *state, GameTextures *textures, Vector2 mouse) {
    // Renderização do painel de estatísticas.
    RenderStatsPanel(&state->stats, textures);

    Vector2 Origin = {0, 0};

    // Iteração sobre o grid para desenhar tiles e personagens.
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            Rectangle tileDest = ScaleRectTo720p(GRID_MARGIN_X + (c * 96), GRID_MARGIN_Y + (r * 78), 96, 78, BASE_WIDTH_INT, BASE_HEIGHT_INT);
            Texture2D tileTexture = (c == TILE_TYPE_BUTTON) ? textures->buttonTile : textures->metallicTile;
            Rectangle tileSource = {0, 0, (float)tileTexture.width, (float)tileTexture.height};
            DrawTexturePro(tileTexture, tileSource, tileDest, Origin, 0.0f, WHITE);

            Character *character = &state->entities.characters[r][c];
            if (character->exists) {
                const CharacterInfo *charInfo = &CHARACTER_INFO[character->type];

                // Pega a textura do frame de animação atual.
                Texture2D charTexture = textures->characterTextures[character->type][character->currentFrame];

                // Posição final do personagem com offsets do grid.
                float posX = GRID_MARGIN_X + (c * 96) + charInfo->destOffset.x;
                float posY = GRID_MARGIN_Y + (r * 78) + charInfo->destOffset.y;

                // Retângulo de destino do personagem.
                Rectangle charDest = ScaleRectTo720p(posX, posY, charInfo->destSize.x, charInfo->destSize.y, BASE_WIDTH_INT, BASE_HEIGHT_INT);

                DrawTexturePro(charTexture, charInfo->sourceRec, charDest, Origin, 0.0f, WHITE);
            }

            // Highlight visual das tiles ao passar o mouse (exceto coluna 0).
            if (c != TILE_TYPE_BUTTON && CheckCollisionPointRec(mouse, tileDest) && !state->app.isPaused) {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                DrawRectangleRec(tileDest, ColorAlpha(YELLOW, 0.3f));
            }
        }
    }
}

// Renderização dos projéteis ativos no jogo.
void RenderProjectiles(GameState *state, GameTextures *textures) {
    Vector2 Origin = {0, 0};
    // A região da spritesheet do projétil.
    Rectangle projectileSource = {5, 5, 71, 29};

    for (int i = 0; i < MAX_PROJECTILES_ON_SCREEN; i++) {
        // Se o projétil estiver ativo, desenha-o.
        if (state->entities.projectiles[i].isActive) {
            // Pega a posição do projétil e a escala para a resolução atual.
            Rectangle projectileDest = ScaleRectTo720p(
                state->entities.projectiles[i].position.x,
                state->entities.projectiles[i].position.y,
                71,  // Largura original da sprite.
                29,  // Altura original da sprite.
                BASE_WIDTH_INT, BASE_HEIGHT_INT);

            DrawTexturePro(textures->projectile, projectileSource, projectileDest, Origin, 0.0f, WHITE);
        }
    }
}

// Renderização das bombas ativas no jogo.
void RenderBombs(GameState *state, GameTextures *textures) {
    Vector2 Origin = {0, 0};
    // A região da spritesheet do projétil.
    Rectangle bombSource = {378, 93, 267, 839};

    for (int i = 0; i < MAX_PROJECTILES_ON_SCREEN; i++) {
        // Se a bomba estiver ativa, desenha-a.
        if (state->entities.bombs[i].isActive) {
            // Pega a posição da bomba e a escala para a resolução atual.
            Rectangle bombDest = ScaleRectTo720p(
                state->entities.bombs[i].position.x,
                state->entities.bombs[i].position.y,
               37.8,  // Largura original da sprite.
               83.9,  // Altura original da sprite.
                BASE_WIDTH_INT, BASE_HEIGHT_INT);

            DrawTexturePro(textures->bomb, bombSource, bombDest, Origin, 0.0f, WHITE);
        }
    }
}


// Renderização da bolsa de dinheiro aleatória.
void RenderMoneyBag(GameState *state, GameTextures *textures, Vector2 mouse) {
    if (!state->moneyBag.isActive) return;  // Só renderiza se a bolsa estiver ativa.

    Rectangle moneyBagSource = {18, 11, 165, 210};  // Região da spritesheet da bolsa.

    float sizeMod = state->moneyBag.isPulsing ? 3.0f : 0.0f;

    // Usa a posição randomizada do GameState
    Rectangle moneyBagDest = ScaleRectTo720p(state->moneyBag.position.x, state->moneyBag.position.y,
                                             78 + sizeMod, 96 + sizeMod, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    DrawTexturePro(textures->moneyIcon, moneyBagSource, moneyBagDest, (Vector2){0, 0}, 0.0f, WHITE);

    // Highlight visual da bolsa ao passar o mouse.
    if (CheckCollisionPointRec(mouse, moneyBagDest) && !state->app.isPaused) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
}

// Renderiza o personagem selecionado ao lado do mouse de forma transparente.
void RenderSelectedCharacterPreview(GameState *state, GameTextures *textures, Vector2 mouse) {
    CharacterType charInHand = state->app.characterInHand;
    // Só renderiza se um personagem estiver selecionado (não for 1 ou SELL_ID).
    if (charInHand > CHAR_TYPE_NONE && charInHand < CHAR_TYPE_COUNT) {
        Vector2 Origin = {0, 0};
        int virtualMouseX = (int)(mouse.x * BASE_WIDTH_FLOAT / BASE_WIDTH_INT);
        int virtualMouseY = (int)(mouse.y * BASE_HEIGHT_FLOAT / BASE_HEIGHT_INT);
        int offsetX = 10, offsetY = 10;

        Rectangle texMSource = {0, 0, textures->characterFrames[2].width, textures->characterFrames[2].height / 1.5f};  // Mesma fonte do seletor.
        Rectangle texMDest = ScaleRectTo720p(virtualMouseX + offsetX, virtualMouseY + offsetY, 78, 96, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        Color Transparency = {255, 255, 255, 128};  // Transparência para o preview.

        // Renderiza o ícone do personagem selecionado.
        DrawTexturePro(textures->characterFrames[charInHand], texMSource, texMDest, Origin, 0.0f, Transparency);
    }
}

// Renderiza o botão de pause, e deixa o fundo escuro.
void RenderPause(GameState *state, GameTextures *textures, Vector2 mouse) {
    if (!state->app.isPaused) return;

    DrawRectangle(0, 0, BASE_WIDTH_INT, BASE_HEIGHT_INT, ColorAlpha(BLACK, 0.85f));

    Vector2 origin = {0, 0};
    Rectangle optionSource = {0, 0, (float)textures->optionFrame.width, (float)textures->optionFrame.height};

    Rectangle resumeButtonDest = ScaleRectTo720p(480, BASE_HEIGHT_FLOAT / 4.0f, 360, 121, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle resumeGlowDest = ScaleRectTo720p(504, (BASE_HEIGHT_FLOAT / 4.0f) + 24, 312, 121 - 48, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    Rectangle exitButtonDest = ScaleRectTo720p(480, BASE_HEIGHT_FLOAT / 2.0f, 360, 121, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle exitGlowDest = ScaleRectTo720p(504, (BASE_HEIGHT_FLOAT / 2.0f) + 24, 312, 121 - 48, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    // Desenha a moldura dos botões.
    DrawTexturePro(textures->optionFrame, optionSource, resumeButtonDest, origin, 0.0f, WHITE);
    DrawTexturePro(textures->optionFrame, optionSource, exitButtonDest, origin, 0.0f, WHITE);

    // Calcula a posição do texto para centralizá-lo nos botões.
    int resumeTextWidth = MeasureText("Resume", FONT_SIZE);
    int exitTextWidth = MeasureText("Exit Game", FONT_SIZE);
    Vector2 resumeTextPos = {resumeButtonDest.x + (resumeButtonDest.width - resumeTextWidth) / 2, resumeButtonDest.y + 35};
    Vector2 exitTextPos = {exitButtonDest.x + (exitButtonDest.width - exitTextWidth) / 2, exitButtonDest.y + 35};

    DrawText("Resume", (int)resumeTextPos.x, (int)resumeTextPos.y, FONT_SIZE, RED);
    DrawText("Exit Game", (int)exitTextPos.x, (int)exitTextPos.y, FONT_SIZE, RED);

    // Lógica de Highlight (brilho) com o mouse.
    if (CheckCollisionPointRec(mouse, resumeGlowDest)) {
        DrawRectangleRec(resumeGlowDest, ColorAlpha(RED, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else if (CheckCollisionPointRec(mouse, exitGlowDest)) {
        DrawRectangleRec(exitGlowDest, ColorAlpha(RED, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void RenderZombies(GameState *state, GameTextures *textures) {
    const float ZOMBIE_RENDER_WIDTH = 72.0f;
    const float ZOMBIE_RENDER_HEIGHT = 90.0f;

    // 2. Tamanho de UM ÚNICO frame DENTRO DA SUA IMAGEM 'zombie.png' (origem).
    //    >>> IMPORTANTE: Você talvez precise ajustar estes valores! <<<
    //    Abra seu arquivo 'zombie.png' em um editor de imagem e veja a largura e altura
    //    de um dos 6 zumbis. Um bom chute inicial é 80x140.
    const float ZOMBIE_SPRITE_FRAME_WIDTH = 340.0f;
    const float ZOMBIE_SPRITE_FRAME_HEIGHT = 550.0f;

    for (int i = 0; i < MAX_ZOMBIES_ON_SCREEN; i++) {
        const Zombie *zombie = &state->entities.zombies[i];
        if (zombie->isActive) {
            Rectangle sourceRec = {
                zombie->currentFrame * ZOMBIE_SPRITE_FRAME_WIDTH,
                0,
                ZOMBIE_SPRITE_FRAME_WIDTH,
                ZOMBIE_SPRITE_FRAME_HEIGHT};

            Rectangle destRec = {
                zombie->position.x,
                zombie->position.y,
                ZOMBIE_RENDER_WIDTH,
                ZOMBIE_RENDER_HEIGHT};

            DrawTexturePro(textures->zombie, sourceRec, destRec, (Vector2){0, 0}, 0, WHITE);
        }
    }
}

void RenderHordeStatus(GameState *state) {
    if (state->horde.state != HORDE_STATE_BETWEEN_WAVES) {
        return;
    }

    char buffer[64];
    int timeRemaining = (int)ceilf(state->horde.spawnTimer);
    sprintf(buffer, "Next Wave in: %d", timeRemaining);

    int fontSize = FONT_SIZE / 1.5;
    int textWidth = MeasureText(buffer, fontSize);

    int padding = 20;
    Vector2 textPos = {
        (float)BASE_WIDTH_FLOAT - textWidth - padding,
        (float)padding};

    DrawText(buffer, textPos.x, textPos.y, fontSize, BLACK);
}