#include "graphics.h"
#include "utils.h"
#include "config.h"
#include <stdio.h>

void InitializeTextures(GameTextures* textures) {
    const char* nomes[5] = {
        "chimpanzini", "tralalero", "sahur", "lirili", "bombardini"
    };

    // Carrega texturas básicas.
    textures->metallicTile = LoadTexture("assets/elements/metallictile.png");
    textures->buttonTile = LoadTexture("assets/elements/buttontilemetallic.png");
    textures->frame = LoadTexture("assets/elements/frame2.png");
    textures->pointsIcon = LoadTexture("assets/elements/points.png");
    textures->projectile = LoadTexture("assets/characters/projectile.png");
    textures->bomb = LoadTexture("assets/characters/bomb.png");

    // Carrega todas as texturas dos personagens automaticamente.
    char path[100];
    for(int t = 0; t < 8; t++) {
        for (int i = 0; i < 5; i++) {
            sprintf(path, "assets/characters/%s%d.png", nomes[i], t);
            textures->characterTextures[i][t] = LoadTexture(path);
        }
    }

    // Texturas que ficam dentro do quadro seletor de personagem.
    textures->characterFrames[0] = LoadTexture("assets/characters/chimpanziniframe.png");
    textures->characterFrames[1] = LoadTexture("assets/characters/tralaleroframe.png");
    textures->characterFrames[2] = LoadTexture("assets/characters/sahurframe.png");
    textures->characterFrames[3] = LoadTexture("assets/characters/liriliframe.png");
    textures->characterFrames[4] = LoadTexture("assets/characters/bombardiniframe.png");
}

void UnloadTextures(GameTextures* textures) {
    UnloadTexture(textures->metallicTile);
    UnloadTexture(textures->buttonTile);
    UnloadTexture(textures->frame);
    UnloadTexture(textures->pointsIcon);
    UnloadTexture(textures->projectile);
    UnloadTexture(textures->bomb);

    for(int t = 0; t < 8; t++) {
        for (int i = 0; i < 5; i++) {
            UnloadTexture(textures->characterTextures[i][t]);
        }
    }

    for (int i = 0; i < 5; i++) {
        UnloadTexture(textures->characterFrames[i]);
    }
}

void RenderTitleScreen(int screenWidth, int screenHeight, int fontSize) {
    Rectangle playDest = ScaleRectTo720p((int)1280/2.5-5, (int)720/2, 210, fontSize, screenWidth, screenHeight);
    Vector2 mouse = GetMousePosition();
    
    DrawText("Todos vs. Jacques", screenWidth/3, screenHeight/3, fontSize, BLACK);
    DrawText("Play Game", screenWidth/2.5, screenHeight/2, fontSize, BLACK);

    if (CheckCollisionPointRec(mouse, playDest)) {
        DrawRectangleRec(playDest, ColorAlpha(YELLOW, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
}

void RenderHUD(GameState* state, int screenWidth, int screenHeight, int fontSize, 
               const GameTextures* textures, Vector2 mouse) {
    Vector2 Origin = { 0, 0 };
    char pointsText[10];
    sprintf(pointsText, "%d", state->points);

    // Mostrador de número de pontos.
    Vector2 textPoints = ScaleTo720p(110, 50, screenWidth, screenHeight);
    float spacing = 10;
    int textWidth = MeasureText(pointsText, fontSize);
    float textHeight = fontSize;

    Rectangle pointsCountDest = {
        textPoints.x + textWidth + spacing,
        textPoints.y + (textHeight / 2.1f) - (35 * screenHeight / 720.0f) / 2.0f,
        35 * screenWidth / 1280.0f,
        35 * screenHeight / 720.0f
    };

    Rectangle pointsCountSource = { 275, 26, 179, 179 };

    // Texto do número de pontos.
    DrawText(pointsText, (int)textPoints.x, (int)textPoints.y, fontSize, BLACK);

    // Ícone de moeda.
    DrawTexturePro(textures->pointsIcon, pointsCountSource, pointsCountDest, Origin, 0.0f, WHITE);

    // Botão de vender.
    const int sellPosX = (int)(750 * screenWidth / 1280.0f);
    const int sellPosY = (int)(40 * screenHeight / 720.0f);
    Rectangle sellDest = ScaleRectTo720p(750-5, 40, 110, 50, screenWidth, screenHeight);
    
    DrawText("SELL", sellPosX, sellPosY, fontSize, BLACK);

    if (CheckCollisionPointRec(mouse, sellDest)) {
        DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }

    if(state->mousePick == SELL_ID) {
        DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
    }
}

void RenderCharacterSelector(GameState* state, int screenWidth, int screenHeight, 
                           int fontSize, const GameTextures* textures, Vector2 mouse) {
    Vector2 Origin = { 0, 0 };
    char costChar[10];

    for (int f = 0; f < 5; f++) {
        Vector2 textValue = ScaleTo720p(310 + (f * 77), 117, screenWidth, screenHeight);

        Rectangle frameDest = ScaleRectTo720p(300 + (f * 77), 20, 78, 96, screenWidth, screenHeight);
        Rectangle frameSource = { 0, 0, textures->frame.width, textures->frame.height };

        Rectangle charFrameDest = ScaleRectTo720p(300 + (f * 77), 29, 78, 82.75f, screenWidth, screenHeight);
        Rectangle charFrameSource = { 0, 0, textures->characterFrames[2].width, textures->characterFrames[2].height / 1.5f };
        
        DrawTexturePro(textures->frame, frameSource, frameDest, Origin, 0.0f, WHITE);

        // Renderiza o custo dos personagens embaixo do quadro deles.
        sprintf(costChar, "%d", state->characterCost[f]);
        DrawTexturePro(textures->characterFrames[f], charFrameSource, charFrameDest, Origin, 0.0f, WHITE);
        DrawText(costChar, 12+(int)textValue.x, (int)textValue.y, fontSize/1.5, BLACK);

        // Highlight do personagem selecionado.
        if (CheckCollisionPointRec(mouse, frameDest)) {
            DrawRectangleRec(frameDest, ColorAlpha(YELLOW, 0.3f));
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }
    }
}

void RenderGameGrid(GameState* state, int screenWidth, int screenHeight, 
                   const GameTextures* textures, Vector2 mouse) {
    Vector2 Origin = { 0, 0 };

    // Regiões das spritesheets.
    Rectangle sahurSource = { 8, 20, 122, 244 };
    Rectangle chimpanziniSource = { 32, 72, 323, 543 };
    Rectangle tralaleroSource = { 13, 57, 186, 144 };
    Rectangle liriliSource = { 35, 19, 190, 225 };
    Rectangle bombardiniSource = { 200, 205, 620, 610 };

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            Rectangle tileDest = ScaleRectTo720p(60 + (c * 96), 220 + (r * 78), 96, 78, screenWidth, screenHeight);
            Rectangle tileSource = { 0, 0, textures->buttonTile.width, textures->buttonTile.height };

            // Renderiza os personagens e Tiles, verificando seu código.
            switch (state->tiles[r][c]) {
                case 0:
                    DrawTexturePro(textures->buttonTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    break;
                case 1:
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    break;
                case CHIMPANZINI_ID: {
                    Rectangle chimpanziniDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 323/5, 543/5, screenWidth, screenHeight);
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    DrawTexturePro(textures->characterTextures[0][state->chimpanzini[r][c].idle], chimpanziniSource, chimpanziniDest, Origin, 0.0f, WHITE);
                    break;
                }
                case TRALALERO_ID: {
                    Rectangle tralaleroDest = ScaleRectTo720p(80 + (c * 96)-20, 220 + (r * 78), 186/2, 144/2, screenWidth, screenHeight);
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    DrawTexturePro(textures->characterTextures[1][state->tralalero[r][c].idle], tralaleroSource, tralaleroDest, Origin, 0.0f, WHITE);
                    break;
                }
                case SAHUR_ID: {
                    Rectangle sahurDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 122/2.5, 244/2.5, screenWidth, screenHeight);
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    DrawTexturePro(textures->characterTextures[2][state->sahur[r][c].idle], sahurSource, sahurDest, Origin, 0.0f, WHITE);
                    break;
                }
                case LIRILI_ID: {
                    Rectangle liriliDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 190/2.5, 225/2.5, screenWidth, screenHeight);
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    DrawTexturePro(textures->characterTextures[3][state->lirili[r][c].idle], liriliSource, liriliDest, Origin, 0.0f, WHITE);
                    break;
                }
                case BOMBARDINI_ID: {
                    Rectangle bombardiniDest = ScaleRectTo720p(80 + (c * 96)-2, 220 + (r * 78) + 9, 620/10, 610/10, screenWidth, screenHeight);
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    DrawTexturePro(textures->characterTextures[4][state->bombardini[r][c].idle], bombardiniSource, bombardiniDest, Origin, 0.0f, WHITE);
                    break;
                }
                default:
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    break;
            }

            // Highlight das tiles.
            if (state->tiles[r][c] != 0 && CheckCollisionPointRec(mouse, tileDest)) {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                DrawRectangleRec(tileDest, ColorAlpha(YELLOW, 0.3f));
            }
        }
    }

    // Renderiza o personagem selecionado ao lado do mouse.
    if(state->mousePick > 15 && state->mousePick < 24) {
        int virtualMouseX = (int)(mouse.x * 1280.0f / screenWidth);
        int virtualMouseY = (int)(mouse.y * 720.0f / screenHeight);
        int offsetX = 10, offsetY = 10;

        Color Transparency = { 255, 255, 255, 128 };
        Rectangle texMSource = { 0, 0, textures->characterFrames[2].width, textures->characterFrames[2].height / 1.5f };
        Rectangle texMDest = ScaleRectTo720p(virtualMouseX + offsetX, virtualMouseY + offsetY, 78, 96, screenWidth, screenHeight);
        DrawTexturePro(textures->characterFrames[state->mousePick-CHIMPANZINI_ID], texMSource, texMDest, Origin, 0.0f, Transparency);
    }
}

void RenderProjectiles(GameState* state, int screenWidth, int screenHeight, 
                      const GameTextures* textures) {
    Vector2 Origin = { 0, 0 };
    Rectangle projectileSource = { 5, 5, 71, 29 };

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            if (state->tralalero[r][c].projecB == true) {
                Rectangle projectileDest = ScaleRectTo720p((int)state->tralalero[r][c].projecX, 
                                                         state->tralalero[r][c].projecY + BOMBARDINI_ID, 
                                                         71, 29, screenWidth, screenHeight);
                DrawTexturePro(textures->projectile, projectileSource, projectileDest, Origin, 0.0f, WHITE);
            }
        }
    }
}

void RenderPointsBag(GameState* state, int screenWidth, int screenHeight, 
                    const GameTextures* textures, Vector2 mouse) {
    if(!state->pointsBag) return;

    Vector2 Origin = { 0, 0 };
    Rectangle pointsBagSource = { 18, 11, 165, 210 };
    
    // TODO: Check this.
    // Calcula posição aleatória (deveria vir do game state)
    int RandomNumX = 100; // Placeholder - deveria ser calculado no game.c
    int RandomNumY = 100; // Placeholder - deveria ser calculado no game.c
    
    Rectangle pointsBagDest = ScaleRectTo720p(RandomNumX, RandomNumY, 78 + state->pisc, 96 + state->pisc, screenWidth, screenHeight);

    DrawTexturePro(textures->pointsIcon, pointsBagSource, pointsBagDest, Origin, 0.0f, WHITE);

    if(CheckCollisionPointRec(mouse, pointsBagDest)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
}
