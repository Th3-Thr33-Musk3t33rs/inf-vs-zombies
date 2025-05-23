#ifndef CONFIG_H
#define CONFIG_H

#define GAME_TITLE "Todos vs. Jacques"

// Configurações de tela.
#define BASE_WIDTH_FLOAT 1280.0f
#define BASE_HEIGHT_FLOAT 720.0f
#define BASE_WIDTH_INT 1280
#define BASE_HEIGHT_INT 720

// Link externo.
extern const int SELL_POS_X;
extern const int SELL_POS_Y;
extern const int FONT_SIZE;

// Configurações do grid.
#define ROWS 7
#define COLUMNS 9
#define GRID_MARGIN_X 418
#define GRID_MARGIN_Y 174


// Características do Chimpanzini.
#define CHIMPANZINI_ID 16
#define CHIMPANZINI_FRAME_ID 0
#define CHIMPAZINI_COST 50

#define TRALALERO_FRAME_ID 1
#define TRALALERO_ID 17
#define TRALALERO_COST 100

#define SAHUR_ID 18
#define SAHUR_FRAME_ID 2
#define SAHUR_COST 150

#define LIRILI_ID 19
#define LIRILI_FRAME_ID 3
#define LIRILI_COST 50

#define BOMBARDINI_ID 20
#define BOMBARDINI_FRAME_ID 4
#define BOMBARDINI_COST 25

// ID especial para venda.
#define SELL_ID 404

// Configurações de jogo.
#define TARGET_FPS 60
#define BASE_FONT_SIZE 40
#define MONEY_BAG_RANDOMNESS 1200

// Velocidades.
#define PROJECTILE_SPEED 400.0f
#define BOMB_SPEED 1200.0f

// Valores de jogo.
#define INITIAL_MONEY 150
#define DAMAGE 10
#define ENEMY_HP 110

#endif
