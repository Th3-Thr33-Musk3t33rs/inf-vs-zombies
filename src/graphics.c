#define _CRT_SECURE_NO_WARNINGS

#include "graphics.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>

#include "character_data.h"
#include "config.h"
#include "raylib.h"
#include "types.h"
#include "utils.h"

Vector2 defaultOrigin = {0, 0};

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
    textures->goldZombie = LoadTexture("assets/characters/goldzombie.png");
    textures->backGround = LoadTexture("assets/elements/background.jpg");
    textures->leaderboardback = LoadTexture("assets/elements/leaderboardback.png");

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

// Descarrega todas as texturas.
void UnloadTextures(GameTextures *textures) {
    UnloadTexture(textures->metallicTile);
    UnloadTexture(textures->buttonTile);
    UnloadTexture(textures->statsFrame);
    UnloadTexture(textures->optionFrame);
    UnloadTexture(textures->frame);
    UnloadTexture(textures->moneyIcon);
    UnloadTexture(textures->projectile);
    UnloadTexture(textures->bomb);
    UnloadTexture(textures->zombie);
    UnloadTexture(textures->goldZombie);

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
    PlayMusicStream(sounds->backgroundMusic);

    sounds->cancelSFX = LoadSound("assets/sfx/cancel.wav");
    sounds->selectSFX = LoadSound("assets/sfx/select.wav");
    sounds->collectSFX = LoadSound("assets/sfx/collect.wav");
    sounds->collectBagSFX = LoadSound("assets/sfx/collectbag.mp3");
    sounds->putSFX = LoadSound("assets/sfx/put.wav");
    sounds->projectileSFX = LoadSound("assets/sfx/projectile.wav");
    sounds->explosionSFX = LoadSound("assets/sfx/explosion.wav");
    sounds->tungSFX = LoadSound("assets/sfx/tung.wav");
    sounds->hitSFX = LoadSound("assets/sfx/hit.wav");
    sounds->eatSFX = LoadSound("assets/sfx/eating.wav");
    sounds->endGameSFX = LoadSound("assets/sfx/end.mp3");
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
        case SOUND_TUNG:
            PlaySound(sounds->tungSFX);
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

// Helper para highlight visual dos botões ao passar o mouse.
void highlightButton(Rectangle glowDest) {
    if (CheckCollisionPointRec(GetMousePosition(), glowDest)) {
        DrawRectangleRec(glowDest, ColorAlpha(YELLOW, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

// Renderiza a tela de título do jogo.
void RenderTitleScreen(int screenWidth, int screenHeight, int fontSize, GameState *state, GameTextures *textures, Vector2 mouse) {
    DrawTexture(textures->backGround, 0, 0, WHITE);

    int glowYOffset = 24;
    float playButtonY = BASE_HEIGHT_FLOAT / 2.45f;
    Rectangle playButtonDest = ScaleRectTo720p(BUTTONS_X_GLOW, playButtonY, BUTTONS_WIDTH_GLOW, BUTTONS_HEIGHT_GLOW, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle playGlowDest = ScaleRectTo720p(BUTTONS_X, playButtonY + glowYOffset, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    float playLeaderboardButtonY = BASE_HEIGHT_FLOAT / 1.7f;
    Rectangle leaderboardButtonDest = ScaleRectTo720p(BUTTONS_X_GLOW, playLeaderboardButtonY, BUTTONS_WIDTH_GLOW, BUTTONS_HEIGHT_GLOW, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle leaderboardGlowDest = ScaleRectTo720p(BUTTONS_X, playLeaderboardButtonY + glowYOffset, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    float exitButtonY = BASE_HEIGHT_FLOAT / 1.3f;
    Rectangle exitButtonDest = ScaleRectTo720p(BUTTONS_X_GLOW, exitButtonY, BUTTONS_WIDTH_GLOW, BUTTONS_HEIGHT_GLOW, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle exitGlowDest = ScaleRectTo720p(BUTTONS_X, exitButtonY + glowYOffset, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    const char *playText = "Play Game";
    const char *leaderboardText = "Leaderboard";
    const char *exitText = "Exit";

    // Calcula a posição do texto para centralizá-lo nos botões e também do título.
    int playTextWidth = MeasureText(playText, FONT_SIZE);
    int leaderboardTextWidth = MeasureText(leaderboardText, FONT_SIZE);
    int exitTextWidth = MeasureText(exitText, FONT_SIZE);

    int gameTitleTextWidth = MeasureText(GAME_TITLE, FONT_SIZE * 2);
    Rectangle gameTitleDest = ScaleRectTo720p(BUTTONS_X_GLOW, BASE_HEIGHT_FLOAT / 1.5f, BUTTONS_WIDTH_GLOW, BUTTONS_HEIGHT_GLOW, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    DrawText(GAME_TITLE, gameTitleDest.x + (gameTitleDest.width - gameTitleTextWidth) / 2, screenHeight / 4, fontSize * 2, WHITE);

    Vector2 playTextPos = {playButtonDest.x + (playButtonDest.width - playTextWidth) / 2, playButtonDest.y + 35};
    Vector2 leaderboardTextPos = {leaderboardButtonDest.x + (leaderboardButtonDest.width - leaderboardTextWidth) / 2, leaderboardButtonDest.y + 35};
    Vector2 exitTextPos = {exitButtonDest.x + (exitButtonDest.width - exitTextWidth) / 2, exitButtonDest.y + 35};

    Rectangle optionSource = {0, 0, (float)textures->optionFrame.width, (float)textures->optionFrame.height};

    // Desenha a moldura dos botões.
    DrawTexturePro(textures->optionFrame, optionSource, playButtonDest, defaultOrigin, 0.0f, WHITE);
    DrawTexturePro(textures->optionFrame, optionSource, leaderboardButtonDest, defaultOrigin, 0.0f, WHITE);
    DrawTexturePro(textures->optionFrame, optionSource, exitButtonDest, defaultOrigin, 0.0f, WHITE);
    DrawText(playText, (int)playTextPos.x, (int)playTextPos.y, FONT_SIZE, PURPLE);
    DrawText(leaderboardText, (int)leaderboardTextPos.x, (int)leaderboardTextPos.y, FONT_SIZE, PURPLE);
    DrawText(exitText, (int)exitTextPos.x, (int)exitTextPos.y, FONT_SIZE, PURPLE);

    highlightButton(playGlowDest);
    highlightButton(leaderboardGlowDest);
    highlightButton(exitGlowDest);
}

// Renderiza o HUD com dinheiro e botão de venda.
void RenderHUD(GameState *state, GameTextures *textures, Vector2 mouse) {
    char moneyText[10];
    sprintf(moneyText, "%d", state->stats.money);

    // Posição do texto de dinheiro.
    Vector2 textmoney = ScaleTo720p(110, 50, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    int textWidth = MeasureText(moneyText, FONT_SIZE);

    // Retângulo de destino para o ícone de moeda.
    Rectangle moneyCountDest = {
        textmoney.x + textWidth + 10,
        textmoney.y + (FONT_SIZE / 2.1f) - (35 * BASE_HEIGHT_INT / BASE_HEIGHT_FLOAT) / 2.0f, 35, 35};
    Rectangle moneyCountSource = {275, 26, 179, 179};  // Região da spritesheet do ícone.

    DrawText(moneyText, (int)textmoney.x, (int)textmoney.y, FONT_SIZE, WHITE);
    DrawTexturePro(textures->moneyIcon, moneyCountSource, moneyCountDest, defaultOrigin, 0.0f, WHITE);

    // Posição e tamanho do botão "SELL".
    Rectangle sellDest = ScaleRectTo720p(SELL_POS_X - 5, SELL_POS_Y, 110, 50, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    DrawText("SELL", SELL_POS_X, SELL_POS_Y, FONT_SIZE, WHITE);

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
    char costText[10];

    for (int f = CHAR_TYPE_CHIMPANZINI; f < CHAR_TYPE_COUNT; f++) {
        const CharacterInfo *charInfo = &CHARACTER_INFO[f];

        // Renderiza quadro base.
        int frameIndex = f - 1;
        Rectangle frameDest = ScaleRectTo720p(300 + (frameIndex * TILE_HEIGHT), 20, TILE_HEIGHT, TILE_WIDTH, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        Rectangle frameSource = {0, 0, textures->frame.width, textures->frame.height};
        DrawTexturePro(textures->frame, frameSource, frameDest, defaultOrigin, 0.0f, WHITE);

        // Renderiza ícone do personagem.
        Rectangle charFrameDest = ScaleRectTo720p(300 + (frameIndex * TILE_HEIGHT), 29, TILE_HEIGHT, 82.75f, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        Rectangle charFrameSource = {0, 0, textures->characterFrames[2].width, textures->characterFrames[2].height / 1.5f};
        DrawTexturePro(textures->characterFrames[f], charFrameSource, charFrameDest, defaultOrigin, 0.0f, WHITE);

        // Renderiza o custo do personagem embaixo do quadro.
        sprintf(costText, "%d", charInfo->cost);
        Vector2 costPos = ScaleTo720p(310 + (frameIndex * TILE_HEIGHT), 117, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        DrawText(costText, 12 + (int)costPos.x, (int)costPos.y, FONT_SIZE / 1.5, WHITE);

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
            DrawRectangleRec(frameDest, ColorAlpha(BLUE, 0.2f));
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
    Rectangle statsFrameDest = ScaleRectTo720p(0, GRID_MARGIN_Y, GRID_MARGIN_X - 10, ROWS * TILE_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle statsSource = {0, 0, textures->statsFrame.width, textures->statsFrame.height};
    DrawTexturePro(textures->statsFrame, statsSource, statsFrameDest, Origin, 0.0f, WHITE);

    char buffer[64];

    // Renderização dos textos das estatísticas.
    sprintf(buffer, "WAVE: %d", stats->currentWave);
    DrawText(buffer, (int)textWavePos.x, (int)textWavePos.y, FONT_SIZE, PURPLE);

    sprintf(buffer, "Characters Bought: %d", stats->charactersBought);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y, FONT_SIZE / 1.5, PURPLE);
    sprintf(buffer, "Characters Sold: %d", stats->charactersSold);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y + 30, FONT_SIZE / 1.5, PURPLE);
    sprintf(buffer, "Characters Lost: %d", stats->charactersLost);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y + 60, FONT_SIZE / 1.5, PURPLE);
    sprintf(buffer, "Enemies Killed: %d", stats->enemiesKilled);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y + 90, FONT_SIZE / 1.5, PURPLE);
    sprintf(buffer, "Bags Collected: %d", stats->moneyBagsCollected);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y + 120, FONT_SIZE / 1.5, PURPLE);
    sprintf(buffer, "Bags Missed: %d", stats->moneyBagsMissed);
    DrawText(buffer, (int)textStatsPos.x, (int)textStatsPos.y + 150, FONT_SIZE / 1.5, PURPLE);

    sprintf(buffer, "Points: %d", stats->currentPoints);
    DrawText(buffer, (int)textPointsPos.x, (int)textPointsPos.y, FONT_SIZE / 2, PURPLE);
}

// Renderiza o grid principal do jogo, incluindo as tiles e personagens.
void RenderGameGrid(GameState *state, GameTextures *textures, Vector2 mouse) {
    DrawTexture(textures->backGround, 0, 0, WHITE);
    RenderStatsPanel(&state->stats, textures);

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            float generalPosX = GRID_MARGIN_X + (col * TILE_WIDTH);
            float generalPosY = GRID_MARGIN_Y + (row * TILE_HEIGHT);
            Rectangle tileDest = ScaleRectTo720p(generalPosX, generalPosY, TILE_WIDTH, TILE_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);
            Texture2D tileTexture = (col == TILE_TYPE_BUTTON) ? textures->buttonTile : textures->metallicTile;
            Rectangle tileSource = {0, 0, (float)tileTexture.width, (float)tileTexture.height};
            DrawTexturePro(tileTexture, tileSource, tileDest, defaultOrigin, 0.0f, WHITE);

            Character *character = &state->entities.characters[row][col];
            if (character->exists) {
                const CharacterInfo *charInfo = &CHARACTER_INFO[character->type];

                Texture2D charTexture = textures->characterTextures[character->type][character->currentFrame];

                // Posição final do personagem com offsets do grid.
                float posX = generalPosX + charInfo->destOffset.x;
                float posY = generalPosY + charInfo->destOffset.y;

                // Retângulo de destino do personagem.
                Rectangle charDest = ScaleRectTo720p(posX, posY, charInfo->destSize.x, charInfo->destSize.y, BASE_WIDTH_INT, BASE_HEIGHT_INT);

                DrawTexturePro(charTexture, charInfo->sourceRec, charDest, defaultOrigin, 0.0f, WHITE);
            }

            // Highlight visual das tiles ao passar o mouse (exceto coluna 0).
            if (col != TILE_TYPE_BUTTON && CheckCollisionPointRec(mouse, tileDest) && !state->app.isPaused) {
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
    Rectangle bombSource = {378, 93, 267, 839};

    for (int i = 0; i < MAX_PROJECTILES_ON_SCREEN; i++) {
        if (state->entities.bombs[i].isActive) {
            Rectangle bombDest = ScaleRectTo720p(
                state->entities.bombs[i].position.x,
                state->entities.bombs[i].position.y,
                37.8,
                83.9,
                BASE_WIDTH_INT, BASE_HEIGHT_INT);

            DrawTexturePro(textures->bomb, bombSource, bombDest, Origin, 0.0f, WHITE);
        }
    }
}

// Renderização da bolsa de dinheiro aleatória.
void RenderMoneyBag(GameState *state, GameTextures *textures, Vector2 mouse) {
    if (!state->moneyBag.isActive) return;

    Rectangle moneyBagSource = {18, 11, 165, 210};  // Região da spritesheet da bolsa.

    float sizeMod = state->moneyBag.isPulsing ? 3.0f : 0.0f;

    // Usa a posição randomizada do GameState.
    Rectangle moneyBagDest = ScaleRectTo720p(state->moneyBag.position.x, state->moneyBag.position.y,
                                             TILE_HEIGHT + sizeMod, TILE_WIDTH + sizeMod, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    DrawTexturePro(textures->moneyIcon, moneyBagSource, moneyBagDest, defaultOrigin, 0.0f, WHITE);

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

        Rectangle texMSource = {0, 0, textures->characterFrames[2].width, textures->characterFrames[2].height / 1.5f};
        Rectangle texMDest = ScaleRectTo720p(virtualMouseX + offsetX, virtualMouseY + offsetY, TILE_HEIGHT, TILE_WIDTH, BASE_WIDTH_INT, BASE_HEIGHT_INT);
        Color Transparency = {255, 255, 255, 128};

        DrawTexturePro(textures->characterFrames[charInHand], texMSource, texMDest, Origin, 0.0f, Transparency);
    }
}

// Renderiza o botão de pause, e deixa o fundo escuro.
void RenderPause(GameState *state, GameTextures *textures, Vector2 mouse) {
    if (!state->app.isPaused) return;

    DrawRectangle(0, 0, BASE_WIDTH_INT, BASE_HEIGHT_INT, ColorAlpha(BLACK, 0.85f));

    Rectangle optionSource = {0, 0, (float)textures->optionFrame.width, (float)textures->optionFrame.height};

    Rectangle resumeButtonDest = ScaleRectTo720p(480, BASE_HEIGHT_FLOAT / 4.0f, 360, 121, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle resumeGlowDest = ScaleRectTo720p(BUTTONS_X, (BASE_HEIGHT_FLOAT / 4.0f) + 24, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    Rectangle exitButtonDest = ScaleRectTo720p(480, BASE_HEIGHT_FLOAT / 2.0f, 360, 121, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle exitGlowDest = ScaleRectTo720p(BUTTONS_X, (BASE_HEIGHT_FLOAT / 2.0f) + 24, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    // Desenha a moldura dos botões.
    DrawTexturePro(textures->optionFrame, optionSource, resumeButtonDest, defaultOrigin, 0.0f, WHITE);
    DrawTexturePro(textures->optionFrame, optionSource, exitButtonDest, defaultOrigin, 0.0f, WHITE);

    // Calcula a posição do texto para centralizá-lo nos botões.
    int resumeTextWidth = MeasureText("Resume", FONT_SIZE);
    int exitTextWidth = MeasureText("Exit Game", FONT_SIZE);
    Vector2 resumeTextPos = {resumeButtonDest.x + (resumeButtonDest.width - resumeTextWidth) / 2, resumeButtonDest.y + 35};
    Vector2 exitTextPos = {exitButtonDest.x + (exitButtonDest.width - exitTextWidth) / 2, exitButtonDest.y + 35};

    DrawText("Resume", (int)resumeTextPos.x, (int)resumeTextPos.y, FONT_SIZE, PURPLE);
    DrawText("Exit Game", (int)exitTextPos.x, (int)exitTextPos.y, FONT_SIZE, PURPLE);

    // Lógica de Highlight (brilho) com o mouse.
    if (CheckCollisionPointRec(mouse, resumeGlowDest)) {
        DrawRectangleRec(resumeGlowDest, ColorAlpha(PURPLE, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else if (CheckCollisionPointRec(mouse, exitGlowDest)) {
        DrawRectangleRec(exitGlowDest, ColorAlpha(PURPLE, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void RenderLeaderboard(GameState *state, GameTextures *textures, Vector2 mouse) {
    DrawTexture(textures->leaderboardback, 0, 0, WHITE);
    int leaderboardTextWidth = MeasureText("VTR", FONT_SIZE);
    char points[18];
    char position[5];
    bool isPlayerScoreEnough = false;

    state->player.points = state->stats.currentPoints;

    Rectangle optionSource = {0, 0, (float)textures->optionFrame.width, (float)textures->optionFrame.height};

    Rectangle backButtonDest = ScaleRectTo720p(BUTTONS_X_GLOW, BASE_HEIGHT_FLOAT / 1.3f, BUTTONS_WIDTH_GLOW, BUTTONS_HEIGHT_GLOW, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle backGlowDest = ScaleRectTo720p(BUTTONS_X, (BASE_HEIGHT_FLOAT / 1.3f) + 24, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    Rectangle saveButtonDest = ScaleRectTo720p(BUTTONS_X_GLOW, BASE_HEIGHT_FLOAT / 1.7f, BUTTONS_WIDTH_GLOW, BUTTONS_HEIGHT_GLOW, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Rectangle saveGlowDest = ScaleRectTo720p(BUTTONS_X, (BASE_HEIGHT_FLOAT / 1.7f) + 24, BUTTONS_WIDTH, BUTTONS_HEIGHT, BASE_WIDTH_INT, BASE_HEIGHT_INT);

    int saveTextWidth = MeasureText("Save", FONT_SIZE);
    Vector2 saveTextPos = {saveButtonDest.x + (saveButtonDest.width - saveTextWidth) / 2, saveButtonDest.y + 35};

    Rectangle leaderboardTextDest = ScaleRectTo720p(520, BASE_HEIGHT_FLOAT / 9.0f, BUTTONS_WIDTH_GLOW, BUTTONS_HEIGHT_GLOW, BASE_WIDTH_INT, BASE_HEIGHT_INT);
    Vector2 leaderboardTextPos = {leaderboardTextDest.x + (leaderboardTextDest.width - leaderboardTextWidth) - (BASE_HEIGHT_FLOAT / 2), leaderboardTextDest.y + 35};

    // Posicao da ultima linha horizontal.
    Vector2 lastStartingPoint = {(int)leaderboardTextPos.x - 10, leaderboardTextPos.y - 10 + ((sizeof(state->leaderboard) / sizeof(state->leaderboard[0])) * 60)};
    Vector2 lastEndPoint = {BASE_WIDTH_FLOAT - ((int)leaderboardTextPos.x - 10) + 80, leaderboardTextPos.y - 10 + ((sizeof(state->leaderboard) / sizeof(state->leaderboard[0])) * 60)};

    int pointsColumnRightEdge = (int)(BASE_WIDTH_FLOAT - ((int)leaderboardTextPos.x - 10) + 80);  // Alinha os pontos na direita

    for (int i = 0; i < MAX_PLAYERS_LEADERBOARD; i++) {
        Vector2 startingPoint = {(int)leaderboardTextPos.x - 10, leaderboardTextPos.y - 10 + (i * 60)};
        Vector2 endPoint = {BASE_WIDTH_FLOAT - ((int)leaderboardTextPos.x - 10) + 80, leaderboardTextPos.y - 10 + (i * 60)};

        switch (i + 1) {
            case 1:
                sprintf(position, "%dst", i + 1);
                break;
            case 2:
                sprintf(position, "%dnd", i + 1);
                break;
            case 3:
                sprintf(position, "%drd", i + 1);
                break;
            default:
                sprintf(position, "%dth", i + 1);
                break;
        }

        // Renderiza as linhas verticais da tabela de leaderboard.
        DrawLineEx(startingPoint, endPoint, 5, VIOLET);

        // Renderiza os nomes, pontos e posicao da lista de leaderboard.
        if (isPlayerScoreEnough && i > 0 && state->app.isGameOver) {
            sprintf(points, "%015d", state->leaderboard[i - 1].points);
            int pointsWidth = MeasureText(points, FONT_SIZE);
            int pointsAlignedX = pointsColumnRightEdge - pointsWidth - 10;  // Alinha os pontos na direita

            DrawText(state->leaderboard[i - 1].playerName, (int)leaderboardTextPos.x, (int)leaderboardTextPos.y + (i * 60), FONT_SIZE, WHITE);
            DrawText(points, pointsAlignedX, (int)leaderboardTextPos.y + (i * 60), FONT_SIZE, WHITE);
        } else if (state->player.points > state->leaderboard[i].points && !isPlayerScoreEnough && state->app.isGameOver) {
            sprintf(points, "%015d", state->player.points);
            int pointsWidth = MeasureText(points, FONT_SIZE);
            int pointsAlignedX = pointsColumnRightEdge - pointsWidth - 10;  // Alinha os pontos na direita

            int key = GetCharPressed();
            while (key > 0) {
                if ((key > ' ') && (key <= 'z') && (state->insertedCharsOnLeaderboard < 3)) {
                    state->player.playerName[state->insertedCharsOnLeaderboard] = toupper((unsigned char)key);
                    state->player.playerName[state->insertedCharsOnLeaderboard + 1] = '\0';
                    state->insertedCharsOnLeaderboard++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && state->insertedCharsOnLeaderboard != 0) {
                state->insertedCharsOnLeaderboard--;
                if (state->insertedCharsOnLeaderboard < 0) {
                    state->insertedCharsOnLeaderboard = 0;
                }
                state->player.playerName[state->insertedCharsOnLeaderboard] = '\0';
            }

            DrawText(state->player.playerName, (int)leaderboardTextPos.x, (int)leaderboardTextPos.y + (i * 60), FONT_SIZE, YELLOW);
            DrawText(points, pointsAlignedX, (int)leaderboardTextPos.y + (i * 60), FONT_SIZE, YELLOW);

            isPlayerScoreEnough = true;
        } else {
            sprintf(points, "%015d", state->leaderboard[i].points);
            int pointsWidth = MeasureText(points, FONT_SIZE);
            int pointsAlignedX = pointsColumnRightEdge - pointsWidth - 10;

            DrawText(state->leaderboard[i].playerName, (int)leaderboardTextPos.x, (int)leaderboardTextPos.y + (i * 60), FONT_SIZE, WHITE);
            DrawText(points, pointsAlignedX, (int)leaderboardTextPos.y + (i * 60), FONT_SIZE, WHITE);
        }
        DrawText(position, (int)leaderboardTextPos.x - 90, (int)leaderboardTextPos.y + (i * 60), FONT_SIZE, WHITE);
    }

    // Renderiza as linhas da tabela de leaderboard.
    Vector2 leftStartingPoint = {(int)leaderboardTextPos.x - 10, leaderboardTextPos.y - 10};
    Vector2 leftEndPoint = {(int)leaderboardTextPos.x - 10, leaderboardTextPos.y - 10 + ((sizeof(state->leaderboard) / sizeof(state->leaderboard[0])) * 60)};
    DrawLineEx(leftStartingPoint, leftEndPoint, 5, VIOLET);

    Vector2 rightStartingPoint = {BASE_WIDTH_FLOAT - ((int)leaderboardTextPos.x - 10) + 80, leaderboardTextPos.y - 10};
    Vector2 rightEndPoint = {BASE_WIDTH_FLOAT - ((int)leaderboardTextPos.x - 10) + 80, leaderboardTextPos.y - 10 + ((sizeof(state->leaderboard) / sizeof(state->leaderboard[0])) * 60)};
    DrawLineEx(rightStartingPoint, rightEndPoint, 5, VIOLET);

    Vector2 middleStartingPoint = {(int)leaderboardTextPos.x + (int)leaderboardTextWidth + 10, leaderboardTextPos.y - 10};
    Vector2 middleEndPoint = {(int)leaderboardTextPos.x + (int)leaderboardTextWidth + 10, leaderboardTextPos.y - 10 + ((sizeof(state->leaderboard) / sizeof(state->leaderboard[0])) * 60)};
    DrawLineEx(middleStartingPoint, middleEndPoint, 5, VIOLET);
    DrawLineEx(lastStartingPoint, lastEndPoint, 5, VIOLET);

    DrawTexturePro(textures->optionFrame, optionSource, backButtonDest, defaultOrigin, 0.0f, WHITE);

    if (!state->app.isGameOver) {
        int backTextWidth = MeasureText("Back", FONT_SIZE);
        Vector2 backTextPos = {backButtonDest.x + (backButtonDest.width - backTextWidth) / 2, backButtonDest.y + 35};
        DrawText("Back", (int)backTextPos.x, (int)backTextPos.y, FONT_SIZE, PURPLE);
    } else {
        int backTextWidth = MeasureText("Title Screen", FONT_SIZE);
        Vector2 backTextPos = {backButtonDest.x + (backButtonDest.width - backTextWidth) / 2, backButtonDest.y + 35};
        DrawText("Title Screen", (int)backTextPos.x, (int)backTextPos.y, FONT_SIZE, PURPLE);

        if (state->insertedCharsOnLeaderboard == 3) {
            DrawTexturePro(textures->optionFrame, optionSource, saveButtonDest, defaultOrigin, 0.0f, WHITE);
        } else {
            Color semiTransparentWHITE = ColorAlpha(WHITE, 0.5f);
            DrawTexturePro(textures->optionFrame, optionSource, saveButtonDest, defaultOrigin, 0.0f, semiTransparentWHITE);
        }

        DrawText("Save", (int)saveTextPos.x, (int)saveTextPos.y, FONT_SIZE, PURPLE);

        if (CheckCollisionPointRec(mouse, saveGlowDest) && state->insertedCharsOnLeaderboard == 3) {
            DrawRectangleRec(saveGlowDest, ColorAlpha(PURPLE, 0.3f));
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
    }

    if (CheckCollisionPointRec(mouse, backGlowDest)) {
        DrawRectangleRec(backGlowDest, ColorAlpha(PURPLE, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void RenderZombies(GameState *state, GameTextures *textures) {
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

            if (zombie->golden) {
                DrawTexturePro(textures->goldZombie, sourceRec, destRec, defaultOrigin, 0, WHITE);
            } else {
                DrawTexturePro(textures->zombie, sourceRec, destRec, defaultOrigin, 0, WHITE);
            }
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
    DrawText(buffer, textPos.x, textPos.y, fontSize, WHITE);
}
