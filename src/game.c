#include "game.h"
#include "utils.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

void InitializeGameState(GameState* state) {
    // Inicializa valores básicos.
    state->titleScreen = true;
    state->gameOver = false;
    state->points = INITIAL_POINTS;
    state->mousePick = 1;
    
    // Inicializa custos dos personagens.
    state->characterCost[0] = 50;  // Chimpanzini.
    state->characterCost[1] = 100; // Tralalero.
    state->characterCost[2] = 150; // Sahur.
    state->characterCost[3] = 50;  // Lirili.
    state->characterCost[4] = 25;  // Bombardini.
    
    // Inicializa contadores.
    state->frameCounterPisc = 0;
    state->frameCounterIdle = 0;
    state->pisc = 0;
    
    // Inicializa bolsa de pontos.
    state->pointsBag = false;
    state->randomize = true;
    state->piscBool = true;
    
    // Seta todas as Tiles como 1, exceto as da coluna 0, que possuem valor 0.
    for(int r = 0; r < ROWS; r++) {
        for(int c = 0; c < COLUMNS; c++) {
            state->tiles[r][c] = 1;
            state->tiles[r][0] = 0;
        }
    }
    
    // Insere os códigos dos personagens no seletor.
    for(int f = CHIMPANZINI_ID; f < 21; f++) {
        state->frame[f-CHIMPANZINI_ID] = f;
    }
    
    // Inicializa arrays de personagens com valores zero.
    memset(state->chimpanzini, 0, sizeof(state->chimpanzini));
    memset(state->tralalero, 0, sizeof(state->tralalero));
    memset(state->sahur, 0, sizeof(state->sahur));
    memset(state->lirili, 0, sizeof(state->lirili));
    memset(state->bombardini, 0, sizeof(state->bombardini));
}

void UpdateGame(GameState* state) {
    // Incrementa FrameCounterIdle no loop principal.
    state->frameCounterIdle++;
    
    // Reseta FrameCounterIdle após 1 minuto.
    if (state->frameCounterIdle == 3600) {
        state->frameCounterIdle = 0;
    }
    
    UpdateCharacters(state);
    
    // Lógica da bolsa de dinheiro aleatória.
    if(state->frameCounterPisc % 18 == 0) {
        state->piscBool = !state->piscBool;
    }
    
    if(state->piscBool == true) {
        state->pisc = 3;
    } else {
        state->pisc = 0;
    }
    
    // Gera bolsa aleatória.
    srand(time(NULL));
    int randomNum = rand() % 20;
    
    if(randomNum == 0 && !state->pointsBag) {
        state->pointsBag = true;
    }
    
    if(state->pointsBag) {
        if(state->randomize == true) {
            state->frameCounterPisc = 3600/2;
        }
        state->randomize = false;
        state->frameCounterPisc--;
        
        if(state->frameCounterPisc <= 0) {
            state->randomize = true;
            state->pointsBag = false;
        }
    }
}
