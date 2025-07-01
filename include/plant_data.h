#ifndef CHARACTER_DATA_H
#define CHARACTER_DATA_H

#include "config.h"
#include "raylib.h"
#include "types.h"

// PlantType é um enum para identificar os tipos de planta.
typedef enum {
    PLANT_TYPE_NONE,
    PLANT_TYPE_SUNFLOWER,
    PLANT_TYPE_PEASHOOTER,
    PLANT_TYPE_CHOMPER,
    PLANT_TYPE_WALLNUT,
    PLANT_TYPE_TACTICAL_CUKE,
    PLANT_TYPE_COUNT,
    PLANT_TYPE_SELL_MODE
} PlantType;

// PlantInfo é uma estrutura que define as propriedades estáticas de uma planta.
typedef struct {
    PlantType type;
    int cost;
    float cooldown;
    int initialHp;
    const char *textureName;
    Rectangle sourceRec;      // Área a ser usada da sprite de animação.
    Vector2 destSize;         // O tamanho que a planta terá na tela.
    Vector2 destOffset;
} PlantInfo;

// PLANT_INFO é um array global e constante que serve como "banco de dados" das características das plantas.
// Ele agrega todas as informações dos personagens do jogo.
// 'extern' diz ao compilador: "Esta variável existe, mas sua definição
// está em outro arquivo .c. Apenas saiba que ela está disponível."
extern const PlantInfo PLANT_INFO[];
#endif
