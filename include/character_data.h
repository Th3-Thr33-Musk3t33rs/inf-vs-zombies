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
    const char *textureName;
    Rectangle sourceRec;      // Área a ser usada da sprite de animação.
    Vector2 destSize;         // O tamanho que o personagem terá na tela.
    Vector2 destOffset;
} CharacterInfo;

// CHARACTER_INFO é um array global e constante que serve como "banco de dados" das características dos personagens.
// Ele agrega todas as informações dos personagens do jogo.
// 'extern' diz ao compilador: "Esta variável existe, mas sua definição
// está em outro arquivo .c. Apenas saiba que ela está disponível."
extern const CharacterInfo CHARACTER_INFO[];
#endif
