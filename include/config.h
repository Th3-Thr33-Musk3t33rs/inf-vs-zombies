#ifndef CONFIG_H
#define CONFIG_H

#define GAME_TITLE "Todos vs. Jacques"

// Configurações de tela.
#define BASE_WIDTH_FLOAT 1280.0f
#define BASE_HEIGHT_FLOAT 720.0f
#define BASE_WIDTH_INT 1280
#define BASE_HEIGHT_INT 720

// TODO: trocar esses 750 e 40 por algo que faça sentido.
// Link externo.
extern const int sell_pos_x;
extern const int sell_pos_y;

// Configurações do grid.
#define ROWS 5
#define COLUMNS 9

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

// Velocidades.
#define PROJECTILE_SPEED 400.0f
#define BOMB_SPEED 1200.0f

// Valores de jogo.
#define INITIAL_POINTS 150
#define DAMAGE 10
#define ENEMY_HP 110

#endif
