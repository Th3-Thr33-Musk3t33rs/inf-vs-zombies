#ifndef CONFIG_H
#define CONFIG_H

#define GAME_TITLE "INF vs Zombies"
#define PAUSE_BUTTONS_X 504
#define PAUSE_BUTTONS_Y_OFFSET 24
#define PAUSE_BUTTONS_WIDTH 312
#define PAUSE_BUTTONS_HEIGHT 73

#define MAX_HORDES 10
#define TIME_BETWEEN_HORDES 1.0f

#define TILE_TYPE_BUTTON 0
#define TILE_TYPE_GRASS 1

#define MAX_PLAYERS_LEADERBOARD 5

#define LEADERBOARD_FILE "top_scores.bin"

// Configurações de tela.
#define BASE_WIDTH_FLOAT 1280.0f
#define BASE_HEIGHT_FLOAT 720.0f
#define BASE_WIDTH_INT 1280
#define BASE_HEIGHT_INT 720
#define X_OFFSET 96
#define Y_OFFSET 78

#define FRAME_CHANGE_RATE 0.2f

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

#define MAX_ZOMBIES_ON_SCREEN 50
#define ZOMBIE_HP 100
#define HORDE_SPAWN_TIMER 12.0f
#define ZOMBIE_SPEED 20.0f
#define ZOMBIE_DAMAGE 15
#define ZOMBIE_KILL_SCORE 100
#define ZOMBIE_DMG_CD_SECONDS 1.0f 
#define REC_ZOMBIE_WIDTH 40
#define REC_ZOMBIE_HEIGHT 80
#define ZOMBIE_RENDER_WIDTH 72.0f
#define ZOMBIE_RENDER_HEIGHT 90.0f
#define ZOMBIE_SPRITE_FRAME_WIDTH 340.0f
#define ZOMBIE_SPRITE_FRAME_HEIGHT 550.0f

#define CHIMPANZINI_ID 16
#define CHIMPANZINI_FRAME_ID 0
#define CHIMPANZINI_COST 50
#define CHIMPANZINI_CD 7.5f
#define CHIMPANZINI_LOOPS_GENERATE 20
#define CHIMPAZINI_INITIAL_HP 20
#define CHIMPAZINI_TEXTURE "chimpanzini"
#define CHIMPAZINI_MONEY_AWARD 25

#define TRALALERO_ID 17
#define TRALALERO_FRAME_ID 1
#define TRALALERO_COST 100
#define TRALALERO_CD 7.5f
#define TRALALERO_INITIAL_HP 50
#define TRALALERO_TEXTURE "tralalero"
#define TRALALERO_PROJECTILE_CD 3
#define PROJECTILE_SPEED 400.0f
#define PROJECTILE_DAMAGE 30

#define SAHUR_ID 18
#define SAHUR_FRAME_ID 2
#define SAHUR_COST 150
#define SAHUR_CD 7.5f
#define SAHUR_INITIAL_HP 50
#define SAHUR_TEXTURE "sahur"
#define SAHUR_LOOPS 30

#define LIRILI_ID 19
#define LIRILI_FRAME_ID 3
#define LIRILI_COST 50
#define LIRILI_CD 30
#define LIRILI_TEXTURE "lirili"
#define LIRILI_INITIAL_HP 300

#define BOMBARDINI_ID 20
#define BOMBARDINI_FRAME_ID 4
#define BOMBARDINI_COST 25
#define BOMBARDINI_CD 30
#define BOMBARDINI_TEXTURE "bombardini"
#define BOMBARDINI_REIMBURSEMENT 10
#define BOMBARDINI_INITIAL_HP 10
#define BOMBARDINI_LOOPS 3

// Configurações de jogo.
#define TARGET_FPS 60
#define BASE_FONT_SIZE 40
#define MONEY_BAG_RANDOMNESS 800
#define MONEY_BAG_AMOUNT 25
#define MAX_PROJECTILES_ON_SCREEN 100
#define REC_PROJ_WIDHT 20
#define REC_PROJ_HEIGHT 20

// Velocidades.
#define BOMB_SPEED 1800.0f

// Valores de jogo.
#define INITIAL_MONEY 10000
#define DEFAULT_REIMBURSEMENT_RATE 0.5f

// Valores dos sons.
#define SOUND_PROJECTILE 1
#define SOUND_EXPLOSION 17
#define SOUND_TUNG 18
#define SOUND_SELECT 2
#define SOUND_COLLECT 3
#define SOUND_COLLECTBAG 4
#define SOUND_CANCEL 5
#define SOUND_PUT 6
#define SOUND_HIT 7
#define SOUND_EAT 8
#define SOUND_END_GAME 9
#define SOUND_BGMUSIC 98

// ReadHordesConfig lê o arquivo de configPath e retorna o array com a configuração de hordas e o número de hordas.
int ReadHordesConfig(char *configPath, int hordes[], int maxHordes);
#endif
