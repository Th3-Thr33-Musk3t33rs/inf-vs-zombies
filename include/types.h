#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include "config.h"

// Struct dos personagens.
typedef struct {
    int hp;
    int idle;
    int loop;
    bool shining;
    bool exists;
} Chimpanzini;

typedef struct {
    int hp;
    int idle;
    int loop;
    float projecX;
    int projecY;
    bool projecB;
    bool attacking;
    bool exists;
} Tralalero;

typedef struct {
    int hp;
    int idle;
    int cooldown;
    bool attacking;
    bool wait;
    bool exists;
} Sahur;

typedef struct {
    int hp;
    int idle;
    bool exists;
} Lirili;

typedef struct {
    int hp;
    int idle;
    int loop;
    int bombX;
    float bombY;
    bool bombB;
    bool ready;
    bool exists;
} Bombardini;

// Struct principal do jogo.
typedef struct {
    // Estado do jogo.
    bool titleScreen;
    bool gameOver;
    int points;
    int mousePick;
    
    // Grid de tiles.
    int tiles[ROWS][COLUMNS];
    int frame[5];
    
    // Arrays de personagens.
    Chimpanzini chimpanzini[ROWS][COLUMNS];
    Tralalero tralalero[ROWS][COLUMNS];
    Sahur sahur[ROWS][COLUMNS];
    Lirili lirili[ROWS][COLUMNS];
    Bombardini bombardini[ROWS][COLUMNS];
    
    // Contadores de frame.
    int frameCounterPisc;
    int frameCounterIdle;
    int pisc;
    
    // Bolsa de pontos aleatória.
    bool pointsBag;
    bool randomize;
    bool piscBool;
    
    // Custos dos personagens.
    int characterCost[5];
} GameState;

// Struct para texturas.
typedef struct {
    Texture2D metallicTile;
    Texture2D buttonTile;
    Texture2D frame;
    Texture2D characterTextures[8][8];
    Texture2D characterFrames[8];
    Texture2D pointsIcon;
    Texture2D projectile;
    Texture2D bomb;
} GameTextures;

#endif
