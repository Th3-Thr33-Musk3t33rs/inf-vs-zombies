#ifndef CHARACTER_DATA_H
#define CHARACTER_DATA_H

#include "config.h"
#include "raylib.h"
#include "types.h"

// CharacterType é um enum para identificar os tipos de personagem.
typedef enum {
    CHAR_TYPE_NONE,
    CHAR_TYPE_CHIMPANZINI,
    CHAR_TYPE_TRALALERO,
    CHAR_TYPE_SAHUR,
    CHAR_TYPE_LIRILI,
    CHAR_TYPE_BOMBARDINI,
    CHAR_TYPE_COUNT,
    CHAR_TYPE_SELL_MODE
} CharacterType;

// CharacterInfo é uma estrutura que define as propriedades estáticas de um personagem.
typedef struct {
    CharacterType type;
    int cost;
    float cooldown;
    int initialHp;
    const char *textureName;  // Usado para carregar as texturas corretas.
    Rectangle sourceRec;      // Área a ser usada da sprite de animação.
    Vector2 destSize;         // O tamanho que o personagem terá na tela.
    Vector2 destOffset;       // Ajuste fino da posição do personagem na tile.
} CharacterInfo;

// CHARACTER_INFO é um array global e constante que serve como "banco de dados" dos personagens.
// Ele agrega todas as informações dos personagens do jogo.
const CharacterInfo CHARACTER_INFO[] = {
    // A ordem aqui deve corresponder ao enum CharacterType em types.h.
    [CHAR_TYPE_NONE] = {CHAR_TYPE_NONE, 0, 0, 0, ""},
    [CHAR_TYPE_CHIMPANZINI] = {CHAR_TYPE_CHIMPANZINI, CHIMPANZINI_COST, CHIMPANZINI_CD, 20, "chimpanzini", {32, 72, 323, 543}, {323 / 5.0f, 543 / 5.0f}, {20, -10}},
    [CHAR_TYPE_TRALALERO] = {CHAR_TYPE_TRALALERO, TRALALERO_COST, TRALALERO_CD, 50, "tralalero", {13, 57, 186, 144}, {186 / 2.0f, 144 / 2.0f}, {0, 0}},
    [CHAR_TYPE_SAHUR] = {CHAR_TYPE_SAHUR, SAHUR_COST, SAHUR_CD, 50, "sahur", {8, 20, 122, 244}, {122 / 2.5f, 244 / 2.5f}, {20, -10}},
    [CHAR_TYPE_LIRILI] = {CHAR_TYPE_LIRILI, LIRILI_COST, LIRILI_CD, 150, "lirili", {35, 19, 190, 225}, {190 / 2.5f, 225 / 2.5f}, {20, -10}},
    [CHAR_TYPE_BOMBARDINI] = {CHAR_TYPE_BOMBARDINI, BOMBARDINI_COST, BOMBARDINI_CD, 10, "bombardini", {200, 205, 620, 610}, {620 / 10.0f, 610 / 10.0f}, {18, 9}},
};

#endif
