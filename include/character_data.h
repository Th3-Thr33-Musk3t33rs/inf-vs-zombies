#ifndef CHARACTER_DATA_H
#define CHARACTER_DATA_H

#include "types.h"

// CharacterType é um enum para identificar os tipos de personagem.
typedef enum {
    CHAR_TYPE_NONE,
    CHAR_TYPE_CHIMPANZINI,
    CHAR_TYPE_TRALALERO,
    CHAR_TYPE_SAHUR,
    CHAR_TYPE_LIRILI,
    CHAR_TYPE_BOMBARDINI,
    CHAR_TYPE_COUNT
} CharacterType;

// CharacterInfo é uma estrutura que define as propriedades estáticas de um personagem.
typedef struct {
    CharacterType type;
    int cost;
    float cooldown;
    int initialHp;
    const char *textureName;  // Usado para carregar as texturas corretas.
} CharacterInfo;

// CHARACTER_INFO é um array global e constante que serve como "banco de dados" dos personagens.
// Ele agrega todas as informações dos personagens do jogo.
const CharacterInfo CHARACTER_INFO[] = {
    // A ordem aqui deve corresponder ao enum CharacterType em types.h.
    [CHAR_TYPE_NONE] = {CHAR_TYPE_NONE, 0, 0, 0, ""},
    [CHAR_TYPE_CHIMPANZINI] = {CHAR_TYPE_CHIMPANZINI, CHIMPANZINI_COST, CHIMPANZINI_CD, 20, "chimpanzini"},
    [CHAR_TYPE_TRALALERO] = {CHAR_TYPE_TRALALERO, TRALALERO_COST, TRALALERO_CD, 50, "tralalero"},
    [CHAR_TYPE_SAHUR] = {CHAR_TYPE_SAHUR, SAHUR_COST, SAHUR_CD, 50, "sahur"},
    [CHAR_TYPE_LIRILI] = {CHAR_TYPE_LIRILI, LIRILI_COST, LIRILI_CD, 150, "lirili"},
    [CHAR_TYPE_BOMBARDINI] = {CHAR_TYPE_BOMBARDINI, BOMBARDINI_COST, BOMBARDINI_CD, 10, "bombardini"},
};

#endif
