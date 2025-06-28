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

PlayerLeaderboard* LoadLeaderboard(char* fileName) {
    PlayerLeaderboard leaderboard[MAX_PLAYERS_LEADERBOARD];
    char vazio[4] = {"000"};

    FILE* arq = fopen(fileName, "rb");

    if (arq == NULL) {
        for (int i = 0; i < MAX_PLAYERS_LEADERBOARD; i++) {
            strcpy(leaderboard[i].playerName, vazio);
            leaderboard[i].points = 0;
            return leaderboard;
        }
    }
    for (int i = 0; i < MAX_PLAYERS_LEADERBOARD; i++) {
        fread(leaderboard, sizeof(PlayerLeaderboard), MAX_PLAYERS_LEADERBOARD, arq);
        fclose(arq);
    }
    fclose(arq);

    return leaderboard;
}

void SaveLeaderboard(char* fileName, PlayerLeaderboard* leaderboard) {
    FILE* arq = fopen(&fileName, "rw");

    for (int i = 0; i < MAX_PLAYERS_LEADERBOARD; i++) {
        fwrite(&leaderboard[i], sizeof(PlayerLeaderboard), MAX_PLAYERS_LEADERBOARD, arq);
    }
    fclose(arq);
}

int TimeToFrames(float timeInSeconds) {
    // Multiplica o tempo em segundos pela taxa de quadros por segundo.
    return (int)(timeInSeconds * TARGET_FPS);
}
