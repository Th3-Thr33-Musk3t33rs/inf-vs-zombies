#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

#include "config.h"

Vector2 ScaleTo720p(float x, float y, int screenWidth, int screenHeight) {
    float baseWidth = BASE_WIDTH_FLOAT;
    float baseHeight = BASE_HEIGHT_FLOAT;
    return (Vector2){
        x * ((float)screenWidth / baseWidth),
        y * ((float)screenHeight / baseHeight)};
}

Rectangle ScaleRectTo720p(float x, float y, float width, float height, int screenWidth, int screenHeight) {
    Vector2 pos = ScaleTo720p(x, y, screenWidth, screenHeight);
    Vector2 size = ScaleTo720p(width, height, screenWidth, screenHeight);
    return (Rectangle){
        pos.x, pos.y, size.x, size.y};
}

void LoadLeaderboard(char* fileName, GameState* state) {
    FILE* arq = fopen(fileName, "rb");

    if (arq == NULL) {
        // Erro na abertura do arquivo zera a entrada no leaderboard.
        for (int i = 0; i < MAX_PLAYERS_LEADERBOARD; i++) {
            strcpy(state->leaderboard[i].playerName, "000");
            state->leaderboard[i].points = 0;
        }
    } else {
        // Leitura do arquivo binário.
        for (int i = 0; i < MAX_PLAYERS_LEADERBOARD; i++) {
            fread(state->leaderboard[i].playerName, sizeof(char), 4, arq);
            fread(&state->leaderboard[i].points, sizeof(int), 1, arq);
        }

        fclose(arq);

        // Atualização da exibição do leaderboard caso o player tenha pontos suficientes.
        for (int i = 0; i < MAX_PLAYERS_LEADERBOARD - 1; i++) {
            for (int s = 0; s < MAX_PLAYERS_LEADERBOARD - 1; s++) {
                if (state->leaderboard[s].points < state->leaderboard[s + 1].points) {
                    PlayerLeaderboard bufferboard = state->leaderboard[s];
                    state->leaderboard[s] = state->leaderboard[s + 1];
                    state->leaderboard[s + 1] = bufferboard;
                }
            }
        }
    }
}

void SaveLeaderboard(char* fileName, GameState* state) {
    FILE* arq = fopen(fileName, "wb");
    bool isPlayerScoreEnough = false;

    // Erro na abertura para escrita do arquivo binário.
    if (arq == NULL) return;

    // Escrita no arquivo na posição respectiva a pontuação do jogador.
    for (int i = 0; i < MAX_PLAYERS_LEADERBOARD; i++) {
        if (isPlayerScoreEnough && i > 0) {
            fwrite(state->leaderboard[i - 1].playerName, sizeof(char), 4, arq);
            fwrite(&state->leaderboard[i - 1].points, sizeof(int), 1, arq);
        } else if (state->player.points > state->leaderboard[i].points && !isPlayerScoreEnough) {
            fwrite(state->player.playerName, sizeof(char), 4, arq);
            fwrite(&state->player.points, sizeof(int), 1, arq);
            isPlayerScoreEnough = true;
        } else {
            fwrite(state->leaderboard[i].playerName, sizeof(char), 4, arq);
            fwrite(&state->leaderboard[i].points, sizeof(int), 1, arq);
        }
    }

    fclose(arq);
}

int TimeToFrames(float timeInSeconds) {
    return (int)(timeInSeconds * TARGET_FPS);
}
