#ifndef CONFIG_H
#define CONFIG_H

#define GAME_TITLE "Todos vs. Jacques"

#define MAX_HORDES 10
#define TIME_BETWEEN_HORDES 10

#define TILE_TYPE_BUTTON 0
#define TILE_TYPE_GRASS 1

// Configurações de tela.
#define BASE_WIDTH_FLOAT 1280.0f
#define BASE_HEIGHT_FLOAT 720.0f
#define BASE_WIDTH_INT 1280
#define BASE_HEIGHT_INT 720

// 'extern' diz ao compilador: "Esta variável existe, mas sua definição
// está em outro arquivo .c. Apenas saiba que ela está disponível."
extern const int SELL_POS_X;
extern const int SELL_POS_Y;
extern const int FONT_SIZE;

// Configurações do grid.
#define ROWS 7
#define COLUMNS 9
#define GRID_MARGIN_X 418
#define GRID_MARGIN_Y 174

#define ZOMBIE_ID 15

#define CHIMPANZINI_ID 16
#define CHIMPANZINI_FRAME_ID 0
#define CHIMPANZINI_COST 50
#define CHIMPANZINI_CD 7.5f

#define TRALALERO_ID 17
#define TRALALERO_FRAME_ID 1
#define TRALALERO_COST 100
#define TRALALERO_CD 7.5f

#define SAHUR_ID 18
#define SAHUR_FRAME_ID 2
#define SAHUR_COST 150
#define SAHUR_CD 7.5f

#define LIRILI_ID 19
#define LIRILI_FRAME_ID 3
#define LIRILI_COST 50
#define LIRILI_CD 30

#define BOMBARDINI_ID 20
#define BOMBARDINI_FRAME_ID 4
#define BOMBARDINI_COST 25
#define BOMBARDINI_CD 30

// Configurações de jogo.
#define TARGET_FPS 60
#define BASE_FONT_SIZE 40
#define MONEY_BAG_RANDOMNESS 800
#define MONEY_BAG_AMOUNT 40
#define MAX_PROJECTILES 100

// Velocidades.
#define PROJECTILE_SPEED 400.0f
#define BOMB_SPEED 1200.0f

// Valores de jogo.
#define INITIAL_MONEY 50
#define DAMAGE 10
#define ENEMY_HP 110

// Valores dos sons
#define SOUND_PROJECTILE 1
#define SOUND_SELECT 2
#define SOUND_COLLECT 3
#define SOUND_COLLECTBAG 4
#define SOUND_CANCEL 5
#define SOUND_PUT 6
#define SOUND_HIT 7
#define SOUND_BGMUSIC 98

// ReadHordesConfig lê o arquivo de configPath e retorna o array com a configuração de hordas e o número de hordas.
int ReadHordesConfig(char *configPath, int hordes[], int maxHordes);

#endif
