#ifndef TYPES_H
#define TYPES_H

#include "config.h"
#include "raylib.h"
#include "character_data.h"

// Character é uma struct genérica dos personagens existentes.
typedef struct {
    bool exists;
    CharacterType type;
    int hp;
    int row;
    int col;
    float animationCounter;  // Controla a animação.
    int currentFrame;        // Frame atual da animação.

    // Union para dados específicos de cada tipo, economizando memória.
    // Para saber mais sobre unions:
    // https://pt.stackoverflow.com/questions/46668/o-que-s%C3%A3o-unions-por-que-utiliz%C3%A1-los-dentro-de-structs
    union {
        struct {
            bool shining;
            int loop;
        } chimpanzini;
        struct {
            bool attacking;
            float projecX;
            float projecY;
            bool projecB;
            int loop;
        } tralalero;
        struct {
            bool ready;
            int loop;
        } bombardini;
        // CASO HAJA MAIS PERSONAGENS COM CAMPOS ESPECÍFICOS, ADICIONAR AQUI.
    } specific;

} Character;

// PlayerStats é uma estrutura focada nas estatísticas do jogador.
typedef struct {
    int money;
    int currentPoints;
    int currentWave;
    int enemiesKilled;
    int charactersBought;
    int charactersSold;
    int charactersLost;
    int moneyBagsCollected;
    int moneyBagsMissed;
} PlayerStats;

// AppState é uma estrutura para o estado geral da aplicação.
typedef struct {
    bool onTitleScreen;
    bool isPaused;
    bool isMusicPaused;
    bool isGameOver;
    CharacterType characterInHand;
} AppState;

typedef struct {
    int horde_number;
    int zombies_alive_in_horde;
} HordeState;

// EntityManager é uma estrutura para gerenciar as entidades do jogo (personagens, zumbis...).
typedef struct {
    Character characters[ROWS][COLUMNS];
    // Adicionar array de zumbis e projéteis aqui futuramente.
} EntityManager;

// MoneyBag é uma estrutura para a bolsa de dinheiro e suas interações.
typedef struct {
    bool isActive;
    bool shouldRandomizePos;
    bool isPulsing;
    int timeRemainingInFrames;
    int pulseCounter;  // Para o efeito de piscar.
    Vector2 position;
} MoneyBag;

// GameState é uma estrutura com  os estados do jogo e principais funções.
typedef struct {
    PlayerStats stats;
    AppState app;
    EntityManager entities;
    MoneyBag moneyBag;
    HordeState currentHorde;
    int finalHorde;

    int tiles[ROWS][COLUMNS];
    float characterCooldowns[CHAR_TYPE_COUNT];

    // Controle de som
    int soundToPlay;
    bool shouldPlaySound;
} GameState;

// GameTextures é uma estrutura de recursos de texturas 2D do jogo.
typedef struct {
    Texture2D metallicTile;
    Texture2D buttonTile;
    Texture2D statsFrame;
    Texture2D optionFrame;
    Texture2D frame;
    Texture2D characterTextures[CHAR_TYPE_COUNT][8];  // [Tipo de personagem][frame].
    Texture2D characterFrames[CHAR_TYPE_COUNT];
    Texture2D moneyIcon;
    Texture2D projectile;
    Texture2D bomb;
    Texture2D zombie;
} GameTextures;

// GameSounds é uma estrutura com os efeitos sonoros do jogo.
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
