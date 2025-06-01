#ifndef TYPES_H
#define TYPES_H

#include "config.h"
#include "raylib.h"

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

typedef struct {
    int hp;
    int idle;
    int speed;
    int dmg;
    bool exists;
} RegularZombie;

typedef struct {
    int horde_number;
    int zombies_alive_in_horde;
} HordeState;

// Player é uma estrutura focada nas características que são atribuídas a um jogador do nosso jogo.
typedef struct {
    int money;
    int currentPoints;
    int currentWave;
} Player;

// AppState é uma estrutura para controle de estado da nossa aplicação.
typedef struct {
//
} AppState;

// Struct principal do jogo.
typedef struct {
    // Variáveis de controle do jogo.
    bool titleScreen;
    bool pause;
    bool musicPaused;
    bool gameOver;
    HordeState current_horde;
    int final_horde;
    int money;
    int mousePick;

    // Grid de tiles.
    int tiles[ROWS][COLUMNS];
    int frame[5];

    // Estatisticas
    int currentWave;
    int enemiesKilled;
    int charactersBought;
    int charactersSold;
    int charactersLost;
    int moneyBagsCollected;
    int moneyBagsMissed;
    int currentPoints;

    // Arrays de personagens.
    Chimpanzini chimpanzini[ROWS][COLUMNS];
    Tralalero tralalero[ROWS][COLUMNS];
    Sahur sahur[ROWS][COLUMNS];
    Lirili lirili[ROWS][COLUMNS];
    Bombardini bombardini[ROWS][COLUMNS];

    // Contadores de frame.
    int frameCounterPisc;
    int frameCounterIdle;
    float frameCounterCD[5];
    int pisc;

    // Bolsa de dinheiro aleatória.
    bool moneyBag;
    bool randomizePointBagPos;
    bool piscBool;

    int randomNumX;
    int randomNumY;

    // Custos dos Personagens.
    int characterCost[5];
    // Cooldown dos Personagens.
    float characterCD[5];
    bool inCooldown[5];

    int soundToPlay;
    bool shouldPlaySound;
} GameState;

// Struct para texturas.
typedef struct {
    Texture2D metallicTile;
    Texture2D buttonTile;
    Texture2D statsFrame;
    Texture2D optionFrame;
    Texture2D frame;
    Texture2D characterTextures[8][8];
    Texture2D characterFrames[8];
    Texture2D moneyIcon;
    Texture2D projectile;
    Texture2D bomb;
    Texture2D zombie;
} GameTextures;

// Struct para sons
typedef struct {
    Sound selectSFX;
    Sound collectSFX;
    Sound projectileSFX;
    Sound putSFX;
    Sound cancelSFX;
    Sound collectBagSFX;
    Sound hitSFX;
    Music backgroundMusic;
} GameSounds;

#endif
