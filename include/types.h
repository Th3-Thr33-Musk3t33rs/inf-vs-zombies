#ifndef TYPES_H
#define TYPES_H

#include "character_data.h"
#include "config.h"
#include "raylib.h"
#include "types.h"

// PlayerLeaderboard é uma estrutura com a leaderboard do jogo.
typedef struct {
    char playerName[MAX_INPUT_NAME + 1];
    int points;
} PlayerLeaderboard;

// Character é uma struct genérica dos personagens existentes.
typedef struct {
    bool exists;
    CharacterType type;
    int hp;
    int row;
    int col;
    float animationCounter;
    int currentFrame;
    struct {
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
        struct {
            bool cooldown;
            int loop;
        } sahur;
    } specific;
} Character;

// ZombieState é um enum que contém os possíveis estados de um zumbi.
typedef enum {
    ZOMBIE_WALKING,
    ZOMBIE_EATING
} ZombieState;

// Zombie é a estrutura com os atributos de um zumbi.
typedef struct {
    bool isActive;
    Vector2 position;
    int hp;
    int row;
    float animationCounter;
    float damageTimer;
    float eatSoundTimer;
    int currentFrame;
    ZombieState state;
    bool golden;    // Todo zumbi tem uma chance de ser dourado.
} Zombie;

// PlayerStats é uma estrutura focada nas estatísticas do jogador.
typedef struct {
    int nameLetterCount;
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
    bool onLeaderboard;
    bool leavingLeaderboard;
    bool onGameOverScreen;
    bool isMusicPaused;
    bool isGameOver;
    bool shouldQuit;
    CharacterType characterInHand; // É o personagem atualmente selecionado da loja (ou modo de venda).
} AppState;

// HordeState é um enum com os possíveis estados de uma horda.
typedef enum {
    HORDE_STATE_SPAWNING,
    HORDE_STATE_WAITING_CLEAR,
    HORDE_STATE_BETWEEN_WAVES,
    HORDE_STATE_INACTIVE
} HordeState;

// Horde é um estrutura que contém os atributos de uma horda.
typedef struct {
    HordeState state;
    int currentHorde;
    int zombiesToSpawnInHorde;
    int remainingZombies;
    float spawnTimer;
} Horde;

// Projectile é uma estrutura que contém os atributos de um projétil.
typedef struct {
    bool isActive;
    Vector2 position;
} Projectile;

// Bomb é uma estrutura que contém os atributos de uma bomba.
typedef struct {
    bool isActive;
    Vector2 position;
    float explosionY;
} Bomb;

// EntityManager é uma estrutura para gerenciar as entidades do jogo (personagens, zumbis...).
typedef struct {
    Character characters[ROWS][COLUMNS];
    Projectile projectiles[MAX_PROJECTILES_ON_SCREEN];
    Bomb bombs[MAX_PROJECTILES_ON_SCREEN];
    Zombie zombies[MAX_ZOMBIES_ON_SCREEN];
} EntityManager;

// MoneyBag é uma estrutura para a bolsa de dinheiro e suas interações.
typedef struct {
    bool isActive;
    bool shouldRandomizePos;
    bool isPulsing;
    int timeRemainingInFrames;
    int pulseCounter;
    Vector2 position;
} MoneyBag;

// SoundsId é um enum de ids para os sons a serem tocados.
typedef enum {
    SOUND_PROJECTILE,
    SOUND_EXPLOSION,
    SOUND_TUNG,
    SOUND_SELECT,
    SOUND_COLLECT,
    SOUND_COLLECTBAG,
    SOUND_CANCEL,
    SOUND_PUT,
    SOUND_HIT,
    SOUND_EAT,
    SOUND_END_GAME,
    SOUND_BGMUSIC,
} SoundsId;

// GameState é uma estrutura com  os estados do jogo e principais funções.
typedef struct {
    PlayerStats stats;
    AppState app;
    EntityManager entities;
    MoneyBag moneyBag;
    Horde horde;
    PlayerLeaderboard leaderboard[MAX_PLAYERS_LEADERBOARD];
    PlayerLeaderboard player;

    int hordes[MAX_HORDES];
    int totalHordes;
    int insertedCharsOnLeaderboard;

    int tiles[ROWS][COLUMNS];
    float characterCooldowns[CHAR_TYPE_COUNT];

    SoundsId soundToPlay;
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
    Texture2D goldZombie;
    Texture2D backGround;
    Texture2D leaderboardback;
} GameTextures;

// GameSounds é uma estrutura com os efeitos sonoros do jogo.
typedef struct {
    Sound selectSFX;
    Sound collectSFX;
    Sound projectileSFX;
    Sound explosionSFX;
    Sound tungSFX;
    Sound putSFX;
    Sound cancelSFX;
    Sound collectBagSFX;
    Sound hitSFX;
    Sound eatSFX;
    Sound endGameSFX;
    Music backgroundMusic;
} GameSounds;
#endif
