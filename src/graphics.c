#define _CRT_SECURE_NO_WARNINGS

#include "graphics.h"
#include "utils.h"
#include "config.h"
#include "raylib.h"
#include <stdio.h> // Para sprintf

// Carrega todas as texturas necessárias para o jogo.
void InitializeTextures(GameTextures* textures) {
    // Nomes base para carregar as texturas dos personagens.
    const char* nomes[5] = {
        "chimpanzini", "tralalero", "sahur", "lirili", "bombardini"
    };

    // Carrega texturas básicas do ambiente e elementos do jogo.
    textures->metallicTile = LoadTexture("assets/elements/metallictile.png"); 
    textures->buttonTile = LoadTexture("assets/elements/buttontilemetallic.png");
    textures->frame = LoadTexture("assets/elements/frame2.png");
    textures->pointsIcon = LoadTexture("assets/elements/points.png");
    textures->projectile = LoadTexture("assets/elements/projectile.png");
    textures->bomb = LoadTexture("assets/elements/bomb.png");

    // Carrega todas as texturas de animação dos personagens automaticamente.
    char path[100];
    for(int i = 0; i < 5; i++) { // Itera sobre os tipos de personagem
        for (int t = 0; t < 8; t++) { // Itera sobre os frames de animação
            sprintf(path, "assets/characters/%s%d.png", nomes[i], t);
            textures->characterTextures[i][t] = LoadTexture(path);
        }
    }

    // Carrega as texturas dos ícones dos personagens para o seletor.
    textures->characterFrames[CHIMPANZINI_FRAME_ID] = LoadTexture("assets/characters/chimpanziniframe.png");
    textures->characterFrames[TRALALERO_FRAME_ID] = LoadTexture("assets/characters/tralaleroframe.png");
    textures->characterFrames[SAHUR_FRAME_ID] = LoadTexture("assets/characters/sahurframe.png");
    textures->characterFrames[LIRILI_FRAME_ID] = LoadTexture("assets/characters/liriliframe.png");
    textures->characterFrames[BOMBARDINI_FRAME_ID] = LoadTexture("assets/characters/bombardiniframe.png");
}

// Descarrega todas as texturas da memória.
void UnloadTextures(GameTextures* textures) {
    UnloadTexture(textures->metallicTile);
    UnloadTexture(textures->buttonTile);
    UnloadTexture(textures->frame);
    UnloadTexture(textures->pointsIcon);
    UnloadTexture(textures->projectile);
    UnloadTexture(textures->bomb);

    // Descarrega as texturas de animação dos personagens.
    for(int i = 0; i < 5; i++) {
        for (int t = 0; t < 8; t++) {
            UnloadTexture(textures->characterTextures[i][t]);
        }
    }

    // Descarrega as texturas dos ícones dos personagens.
    for (int i = 0; i < 5; i++) {
        UnloadTexture(textures->characterFrames[i]);
    }
}

// Renderiza a tela de título do jogo.
void RenderTitleScreen(int screenWidth, int screenHeight, int fontSize) {
    // Posição e tamanho do botão "Play Game" para detecção de colisão.
    Rectangle playDest = ScaleRectTo720p((int)1280/2.5-5, (int)720/2, 210, fontSize, screenWidth, screenHeight);
    Vector2 mouse = GetMousePosition(); // Pega a posição do mouse para highlight

    DrawText(GAME_TITLE, screenWidth/3, screenHeight/3, fontSize, BLACK);
    DrawText("Play Game", screenWidth/2.5, screenHeight/2, fontSize, BLACK);

    // Highlight visual do botão "Play Game" ao passar o mouse.
    if (CheckCollisionPointRec(mouse, playDest)) {
        DrawRectangleRec(playDest, ColorAlpha(YELLOW, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
}

// Renderiza o HUD (Heads-Up Display) com pontos e botão de venda.
void RenderHUD(GameState* state, int screenWidth, int screenHeight, int fontSize, 
               const GameTextures* textures, Vector2 mouse) {
    Vector2 Origin = { 0, 0 }; // Ponto de origem para DrawTexturePro
    char pointsText[10];
    sprintf(pointsText, "%d", state->points); // Converte a pontuação para string

    // Posição do texto de pontos.
    Vector2 textPoints = ScaleTo720p(110, 50, screenWidth, screenHeight);
    float spacing = 10;
    int textWidth = MeasureText(pointsText, fontSize);
    float textHeight = fontSize;

    // Retângulo de destino para o ícone de moeda.
    Rectangle pointsCountDest = {
        textPoints.x + textWidth + spacing,
        textPoints.y + (textHeight / 2.1f) - (35 * screenHeight / 720.0f) / 2.0f,
        35 * screenWidth / 1280.0f,
        35 * screenHeight / 720.0f
    };
    Rectangle pointsCountSource = { 275, 26, 179, 179 }; // Região da spritesheet do ícone

    // Renderiza o texto do número de pontos.
    DrawText(pointsText, (int)textPoints.x, (int)textPoints.y, fontSize, BLACK);

    // Renderiza o ícone de moeda.
    DrawTexturePro(textures->pointsIcon, pointsCountSource, pointsCountDest, Origin, 0.0f, WHITE);

    // Posição e tamanho do botão "SELL".
    // Usamos as constantes de config.h para a posição.
    Rectangle sellDest = ScaleRectTo720p(SELL_POS_X - 5, SELL_POS_Y, 110, 50, screenWidth, screenHeight);
    
    DrawText("SELL", SELL_POS_X, SELL_POS_Y, fontSize, BLACK);

    // Highlight visual do botão "SELL" ao passar o mouse.
    if (CheckCollisionPointRec(mouse, sellDest)) {
        DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }

    // Highlight se o modo de venda estiver ativo.
    if(state->mousePick == SELL_ID) {
        DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
    }
}

// Renderiza o seletor de personagens na parte superior da tela.
void RenderCharacterSelector(GameState* state, int screenWidth, int screenHeight, 
                           int fontSize, const GameTextures* textures, Vector2 mouse) {
    Vector2 Origin = { 0, 0 };
    char costChar[10]; // Para exibir o custo do personagem

    for (int f = 0; f < 5; f++) { // Itera sobre os 5 slots de personagem no seletor
        Vector2 textValue = ScaleTo720p(310 + (f * 77), 117, screenWidth, screenHeight);

        Rectangle frameDest = ScaleRectTo720p(300 + (f * 77), 20, 78, 96, screenWidth, screenHeight);
        Rectangle frameSource = { 0, 0, textures->frame.width, textures->frame.height };

        Rectangle charFrameDest = ScaleRectTo720p(300 + (f * 77), 29, 78, 82.75f, screenWidth, screenHeight);
        // A fonte da textura do frame do personagem é a mesma para todos os ícones no seletor,
        // então podemos usar um índice fixo como 2 (Sahur) para pegar as dimensões.
        Rectangle charFrameSource = { 0, 0, textures->characterFrames[2].width, textures->characterFrames[2].height / 1.5f };
        
        // Renderiza o quadro do seletor.
        DrawTexturePro(textures->frame, frameSource, frameDest, Origin, 0.0f, WHITE);

        // Renderiza o ícone do personagem dentro do quadro.
        DrawTexturePro(textures->characterFrames[f], charFrameSource, charFrameDest, Origin, 0.0f, WHITE);
        
        // Renderiza o custo do personagem embaixo do quadro.
        sprintf(costChar, "%d", state->characterCost[f]);
        DrawText(costChar, 12+(int)textValue.x, (int)textValue.y, fontSize/1.5, BLACK);

        // Highlight visual do personagem selecionado ao passar o mouse.
        if (CheckCollisionPointRec(mouse, frameDest)) {
            DrawRectangleRec(frameDest, ColorAlpha(YELLOW, 0.3f));
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }
        // Highlight se este personagem estiver atualmente selecionado.
        if (state->mousePick == state->frame[f]) {
            DrawRectangleRec(frameDest, ColorAlpha(BLUE, 0.2f)); // Um highlight diferente para o selecionado
        }
    }
}

// Renderiza o grid principal do jogo, incluindo as tiles e personagens.
void RenderGameGrid(GameState* state, int screenWidth, int screenHeight, 
                   const GameTextures* textures, Vector2 mouse) {
    Vector2 Origin = { 0, 0 };

    // Regiões das spritesheets para os personagens no grid.
    Rectangle sahurSource = { 8, 20, 122, 244 };
    Rectangle chimpanziniSource = { 32, 72, 323, 543 };
    Rectangle tralaleroSource = { 13, 57, 186, 144 };
    Rectangle liriliSource = { 35, 19, 190, 225 };
    Rectangle bombardiniSource = { 200, 205, 620, 610 };

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            Rectangle tileDest = ScaleRectTo720p(60 + (c * 96), 220 + (r * 78), 96, 78, screenWidth, screenHeight);
            Rectangle tileSource = { 0, 0, textures->buttonTile.width, textures->buttonTile.height };

            // Renderiza as Tiles e os personagens baseados no código da tile.
            switch (state->tiles[r][c]) {
                case 0: // Tile de botão (coluna 0)
                    DrawTexturePro(textures->buttonTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    break;
                case 1: // Tile de gramado padrão
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    break;
                case CHIMPANZINI_ID: {
                    // Renderiza a tile e o Chimpanzini se ele existir
                    Rectangle chimpanziniDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 323/5, 543/5, screenWidth, screenHeight);
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    if (state->chimpanzini[r][c].exists) {
                        DrawTexturePro(textures->characterTextures[CHIMPANZINI_FRAME_ID][state->chimpanzini[r][c].idle], chimpanziniSource, chimpanziniDest, Origin, 0.0f, WHITE);
                    }
                    break;
                }
                case TRALALERO_ID: {
                    // Renderiza a tile e o Tralalero se ele existir
                    Rectangle tralaleroDest = ScaleRectTo720p(80 + (c * 96)-20, 220 + (r * 78), 186/2, 144/2, screenWidth, screenHeight);
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    if (state->tralalero[r][c].exists) {
                        DrawTexturePro(textures->characterTextures[TRALALERO_FRAME_ID][state->tralalero[r][c].idle], tralaleroSource, tralaleroDest, Origin, 0.0f, WHITE);
                    }
                    break;
                }
                case SAHUR_ID: {
                    // Renderiza a tile e o Sahur se ele existir
                    Rectangle sahurDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 122/2.5, 244/2.5, screenWidth, screenHeight);
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    if (state->sahur[r][c].exists) {
                        DrawTexturePro(textures->characterTextures[SAHUR_FRAME_ID][state->sahur[r][c].idle], sahurSource, sahurDest, Origin, 0.0f, WHITE);
                    }
                    break;
                }
                case LIRILI_ID: {
                    // Renderiza a tile e o Lirili se ele existir
                    Rectangle liriliDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 190/2.5, 225/2.5, screenWidth, screenHeight);
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    if (state->lirili[r][c].exists) {
                        DrawTexturePro(textures->characterTextures[LIRILI_FRAME_ID][state->lirili[r][c].idle], liriliSource, liriliDest, Origin, 0.0f, WHITE);
                    }
                    break;
                }
                case BOMBARDINI_ID: {
                    // Renderiza a tile e o Bombardini se ele existir
                    Rectangle bombardiniDest = ScaleRectTo720p(80 + (c * 96)-2, 220 + (r * 78) + 9, 620/10, 610/10, screenWidth, screenHeight);
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    if (state->bombardini[r][c].exists) {
                        DrawTexturePro(textures->characterTextures[BOMBARDINI_FRAME_ID][state->bombardini[r][c].idle], bombardiniSource, bombardiniDest, Origin, 0.0f, WHITE);
                    }
                    break;
                }
                default: // Qualquer outro valor (ex: se uma tile for inválida, renderiza como padrão)
                    DrawTexturePro(textures->metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
                    break;
            }

            // Highlight visual das tiles ao passar o mouse (exceto coluna 0).
            if (state->tiles[r][c] != 0 && CheckCollisionPointRec(mouse, tileDest)) {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                DrawRectangleRec(tileDest, ColorAlpha(YELLOW, 0.3f));
            }
        }
    }
}

// Renderização dos projéteis ativos no jogo.
void RenderProjectiles(GameState* state, int screenWidth, int screenHeight, 
                      const GameTextures* textures) {
    Vector2 Origin = { 0, 0 };
    Rectangle projectileSource = { 5, 5, 71, 29 }; // Região da spritesheet do projétil

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            if (state->tralalero[r][c].projecB) { // Se o projétil do Tralalero estiver ativo
                Rectangle projectileDest = ScaleRectTo720p((int)state->tralalero[r][c].projecX, 
                                                         state->tralalero[r][c].projecY + BOMBARDINI_ID, // BOMBARDINI_ID é um offset, talvez não seja o ideal aqui
                                                         71, 29, screenWidth, screenHeight);
                DrawTexturePro(textures->projectile, projectileSource, projectileDest, Origin, 0.0f, WHITE);
            }
            // Lógica para renderizar bombas do Bombardini, se existirem
            // if (state->bombardini[r][c].bombB) { ... }
        }
    }
}

// Renderização da bolsa de pontos aleatória.
void RenderPointsBag(GameState* state, int screenWidth, int screenHeight, 
                    const GameTextures* textures, Vector2 mouse) {
    if(!state->pointsBag) return; // Só renderiza se a bolsa estiver ativa

    Vector2 Origin = { 0, 0 };
    Rectangle pointsBagSource = { 18, 11, 165, 210 }; // Região da spritesheet da bolsa

    // Usa a posição randomizada do GameState
    Rectangle pointsBagDest = ScaleRectTo720p(state->randomNumX, state->randomNumY, 78 + state->pisc, 96 + state->pisc, screenWidth, screenHeight);

    DrawTexturePro(textures->pointsIcon, pointsBagSource, pointsBagDest, Origin, 0.0f, WHITE);

    // Highlight visual da bolsa ao passar o mouse.
    if(CheckCollisionPointRec(mouse, pointsBagDest)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
}

// Renderiza o personagem selecionado ao lado do mouse de forma transparente.
void RenderSelectedCharacterPreview(GameState* state, const GameTextures* textures, Vector2 mouse, int screenWidth, int screenHeight) {
    // Só renderiza se um personagem estiver selecionado (não for 1 ou SELL_ID).
    if (state->mousePick >= CHIMPANZINI_ID && state->mousePick <= BOMBARDINI_ID) {
        Vector2 Origin = { 0, 0 };
        int virtualMouseX = (int)(mouse.x * BASE_WIDTH_FLOAT / screenWidth);
        int virtualMouseY = (int)(mouse.y * BASE_HEIGHT_FLOAT / screenHeight);
        int offsetX = 10, offsetY = 10;

        Color Transparency = { 255, 255, 255, 128 }; // Transparência para o preview
        Rectangle texMSource = { 0, 0, textures->characterFrames[2].width, textures->characterFrames[2].height / 1.5f }; // Mesma fonte do seletor
        Rectangle texMDest = ScaleRectTo720p(virtualMouseX + offsetX, virtualMouseY + offsetY, 78, 96, screenWidth, screenHeight);
        
        // Renderiza o ícone do personagem selecionado.
        DrawTexturePro(textures->characterFrames[state->mousePick - CHIMPANZINI_ID], texMSource, texMDest, Origin, 0.0f, Transparency);
    }
}
