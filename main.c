/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute 'raylib_compile_execute' script
*   Note that compiled executable is placed in the same folder as .c file
*
*   To test the examples on Web, press F6 and execute 'raylib_compile_execute_web' script
*   Web version of the program is generated in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BASE_WIDTH 1280.0f
#define BASE_HEIGHT 720.0f
#define ROWS 5
#define COLUMNS 9

#define CHIMPANZINI 16
#define TRALALERO 17
#define SAHUR 18
#define LIRILI 19
#define BOMBARDINI 20

// Função para manter o HUD igual independente da resolução.
Vector2 ScaleTo720p(float x, float y, int screenWidth, int screenHeight) {   
	float baseWidth = BASE_WIDTH;
	float baseHeight = BASE_HEIGHT;
	return (Vector2) {
		x * ((float)screenWidth / baseWidth), y * ((float)screenHeight / baseHeight)
	};
}

// Função para manter o HUD igual independente da resolução (para Retângulos).
Rectangle ScaleRectTo720p(float x, float y, float width, float height, int screenWidth, int screenHeight) {  
	Vector2 pos = ScaleTo720p(x, y, screenWidth, screenHeight);
	Vector2 size = ScaleTo720p(width, height, screenWidth, screenHeight);
	return (Rectangle) {
		pos.x, pos.y, size.x, size.y
	};
}

int main(void) {
	// Definição de constantes locais.
	const int screenWidth = 1280;
	const int screenHeight = 720;
	const int sellPosX = (int)(750 * screenWidth / 1280.0f);
    const int sellPosY = (int)(40 * screenHeight / 720.0f);
	const float scaleX = (float)screenWidth / BASE_WIDTH;
	const float scaleY = (float)screenHeight / BASE_HEIGHT;

	const char* nomes[5] = {
		"chimpanzini", "tralalero", "sahur", "lirili",
		"bombardini"
	};


	SetTargetFPS(60); // Seta os Quadros por Segundo em 60.

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	//--------------------------------------------------------------------------------------

	Texture2D metallicTile, buttonTile, frame, CharacterTextures[8][8], CharacterFrames[8], pointsIcon, Projectile, Bomb;
	metallicTile = LoadTexture("metallictile.png"); // Textura das Tiles Padrão.
	buttonTile = LoadTexture("buttontilemetallic.png"); // Textura das Tiles de Botão.
	frame = LoadTexture("frame2.png"); // Textura do quadro seletor de personagem.
	pointsIcon = LoadTexture("Points.png");  // Ícone de moeda.
   
    // Carrega todas as texturas dos personagens automaticamente.
	char path[100];
	for(int t = 0; t < 8; t++) {
		for (int i = 0; i < 5; i++) {
			sprintf(path, "Characters/%s%d.png", nomes[i], t);
			CharacterTextures[i][t] = LoadTexture(path);
		}
	}

    // Texturas que ficam dentro do quadro seletor de personagem.
	CharacterFrames[0] = LoadTexture("Characters/chimpanziniframe.png");
	CharacterFrames[1] = LoadTexture("Characters/tralaleroframe.png");
	CharacterFrames[2] = LoadTexture("Characters/sahurframe.png");
	CharacterFrames[3] = LoadTexture("Characters/liriliframe.png");
	CharacterFrames[4] = LoadTexture("Characters/bombardiniframe.png");

    // Textura do projétil do Tralalero.
	Projectile = LoadTexture("Characters/projectile.png");
    
	// Textura da bomba do Bombardini.
	Bomb = LoadTexture("Characters/bomb.png");


    // Array dos custos dos personagens.
	int Tiles[ROWS][COLUMNS], Frame[5], CharacterCost[5];
	CharacterCost[0] = 50;
	CharacterCost[1] = 100;
	CharacterCost[2] = 150;
	CharacterCost[3] = 50;
	CharacterCost[4] = 25;
     
    // Inicialização de Variáveis.
	int MousePick = 1, Points = 150, Damage = 10, EnemieHP = 110;
	
	char PointsT[10], CostChar[10], RandomT[10];
    
    // Struct dos personagens.
	typedef struct Chimpanzini {
		int hp;
		int idle;
		int loop;
		bool shining;
		bool exists;
	} Chimpanzini;

	typedef struct Tralalero {
		int hp;
		int idle;
		int loop;
		float projecX;
		int projecY;
		bool projecB;
		bool attacking;
		bool exists;
	} Tralalero;

	typedef struct Sahur {
		int hp;
		int idle;
		int cooldown;
		bool attacking;
		bool wait;
		bool exists;
	} Sahur;

	typedef struct Lirili {
		int hp;
		int idle;
		bool exists;
	} Lirili;

	typedef struct Bombardini {
		int hp;
		int idle;
		int loop;
		int bombX;
		float bombY;
		bool bombB;
		bool ready;
		bool exists;
	} Bombardini;
    
    // Inicialização das Structs dos personagens.
    Chimpanzini chimpanzini[ROWS][COLUMNS] = {{{ 0, 0, 0, false, false }}};
	Tralalero tralalero[ROWS][COLUMNS] = {{{ 0, 0, 0, 0, 0, false, false, false }}};
	Sahur sahur[ROWS][COLUMNS] = {{{ 0, 0, false, 0,false, false }}};
	Lirili lirili[ROWS][COLUMNS] = {{{ 0, 0, false,}}};
	Bombardini bombardini[ROWS][COLUMNS] = {{{ 0, 0, 0, 0, 0, false, false, false}}};
    
    // FrameCounterPisc e pisc servem para a bolsa de dinheiro aparecer e sumir caso não clicada a tempo, e também para ela piscar.
    // FrameCounterIdle serve para rodar as animações dos personagens.
	int FrameCounterPisc = 0, pisc = 0, FrameCounterIdle = 0;
    
    // piscBool serve para a bolsa de dinheiro piscar.
    // Randomize faz a bolsa ficar parada no lugar quando ela aparecer.
	bool PointsBag = false, Randomize = true, piscBool = true, titleScreen = true, gameOver = false;
    
    // Velocidade do projétil do Tralalero e da bomba do Bombardini.
    float projectileSpeed = 400.0f, bombSpeed = 1200.0f;
    
    // Seta todas as Tiles como 1, exceto as da coluna 0, que possuem valor 0.
	for(int r = 0; r < ROWS; r++) {
		for(int c = 0; c < COLUMNS; c++) {
		    Tiles[r][c] = 1;
			Tiles[r][0] = 0;
		}
	}

    // Insere os códigos dos personagens no seletor.
	for(int f = CHIMPANZINI; f < 21; f++) {
		Frame[f-CHIMPANZINI] = f;
	}
    
	int baseFontSize = 40;
	int fontSize = (int)(baseFontSize * ((float)screenHeight / 720.0f));

	while (!WindowShouldClose()) {
		// Detecta a posição do mouse.
        Vector2 mouse = GetMousePosition(), Origin = { 0, 0 };
		int virtualMouseX = (int)(mouse.x * 1280.0f / screenWidth);
		int virtualMouseY = (int)(mouse.y * 720.0f / screenHeight);

        // Incremente FrameCounterIdle no loop principal.
		FrameCounterIdle++;

        // Reseta FrameCounterIdle após 1 minuto.
		if (FrameCounterIdle == 3600) {
			FrameCounterIdle = 0;
		}

		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		srand(time(NULL));
        
		// Variaveis para fazer a bolsa aparecer em locais e momentos aleatórios.
		int RandomNumY;
		int RandomNumX;
		int RandomNum;
        
		// Randomiza as varíaveis.
		if(Randomize == true) {
			RandomNum = rand() % 20;
			RandomNumY = rand() % (screenHeight-50);
			RandomNumX = rand() % (screenWidth-50);
		}


        // Pega as regiões certas das spritesheets.
		Rectangle sahurSource = { 8, 20, 122, 244 };
		Rectangle chimpanziniSource = { 32, 72, 323, 543 };
		Rectangle tralaleroSource = { 13, 57, 186, 144 };
		Rectangle projectileSource = { 5, 5, 71,29 };
		Rectangle liriliSource = { 35, 19, 190, 225 };
		Rectangle bombardiniSource = { 200, 205, 620, 610 };
		Rectangle pointsCountSource = { 275, 26, 179, 179 };
	
		BeginDrawing();
        ClearBackground(RAYWHITE);

        // Encerra o Jogo e vai pra tela de fim de jogo.
        if(!gameOver) {
			// Tela de Título antes do jogo começar.
			if(titleScreen) {
				// Título do Jogo e Botão de Iniciar Jogo.
				Rectangle playDest = ScaleRectTo720p( (int)1280/2.5-5, (int)720/2, 210, fontSize, screenWidth, screenHeight);
				DrawText("Todos vs. Jacques", screenWidth/3, screenHeight/3, fontSize, BLACK);
				DrawText("Play Game", screenWidth/2.5, screenHeight/2, fontSize, BLACK);

				if (CheckCollisionPointRec(mouse, playDest)) {
					DrawRectangleRec(playDest, ColorAlpha(YELLOW, 0.3f));
					SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
					if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
						titleScreen = false;
					}
				}
			} else {
				// Variáveis para posicionar o personagem selecionado ao lado do mouse.
				int offsetX = 10;
				int offsetY = 10;

				// Mostrador de Número de pontos, o "Dinheiro".
				Vector2 textPoints = ScaleTo720p(110, 50, screenWidth, screenHeight);
				float spacing = 10;

				sprintf(PointsT, "%d", Points);

				sprintf(RandomT, "%d", RandomNum);

				int textWidth = MeasureText(PointsT, fontSize);
				float textHeight = fontSize;
				DrawText(RandomT, 100, 80, fontSize, BLACK);
				Rectangle pointsCountDest = {
					textPoints.x + textWidth + spacing,
					textPoints.y + (textHeight / 2.1f) - (35 * screenHeight / 720.0f) / 2.0f,
					35 * screenWidth / 1280.0f,
					35 * screenHeight / 720.0f
				};

				// Texto do Número de pontos, o "Dinheiro".
				DrawText(PointsT, (int)textPoints.x, (int)textPoints.y, fontSize, BLACK);
				
				// Ícone de Moeda.
				DrawTexturePro(pointsIcon, pointsCountSource, pointsCountDest, Origin, 0.0f, WHITE);

        
				// Botão/Texto de vender.
				Rectangle sellDest = ScaleRectTo720p(750-5, 40, 110, 50, screenWidth, screenHeight);
				DrawText("SELL", sellPosX, sellPosY, fontSize, BLACK);

				if (CheckCollisionPointRec(mouse, sellDest)) {
					DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
					SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
						if(MousePick != 404) {
							MousePick = 404;
						} else {
							MousePick = 1;
						}
					}
				}

        		// Se Clicado, define como selecionado o valor 404 (o código de venda).
				if(MousePick == 404) {
					DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
				}
       
				// For para inserir personagens no quadro de seleção.
				for (int f = 0; f < 5; f++) {
					Vector2 textValue = ScaleTo720p(310 + (f * 77), 117, screenWidth, screenHeight);

					Rectangle frameDest = ScaleRectTo720p(300 + (f * 77), 20, 78, 96, screenWidth, screenHeight);
					Rectangle frameSource = { 0, 0, frame.width, frame.height };

					Rectangle charFrameDest = ScaleRectTo720p(300 + (f * 77), 29, 78, 82.75f, screenWidth, screenHeight);
					Rectangle charFrameSource = { 0, 0, CharacterFrames[2].width, CharacterFrames[2].height / 1.5f };
					DrawTexturePro(frame, frameSource, frameDest, Origin, 0.0f, WHITE);

					// Renderiza o custo dos personagens embaixo do quadro deles.
					sprintf(CostChar, "%d", CharacterCost[f]);
					DrawTexturePro(CharacterFrames[f], charFrameSource, charFrameDest, Origin, 0.0f, WHITE);
					DrawText(CostChar, 12+(int)textValue.x, (int)textValue.y, fontSize/1.5, BLACK);


					// Define o código do personagem escolhido como selecionado.
					if (CheckCollisionPointRec(mouse, frameDest)) {
						DrawRectangleRec(frameDest, ColorAlpha(YELLOW, 0.3f));
						SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
						if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
							MousePick = Frame[f];
						}
					}
				}

				// For Principal.
				for (int r = 0; r < ROWS; r++) {
					for (int c = 0; c < COLUMNS; c++) {
						// Define Tamanho e região do spritesheet das Tiles.
						Rectangle tileDest = ScaleRectTo720p(60 + (c * 96), 220 + (r * 78), 96, 78, screenWidth, screenHeight);
						Rectangle tileSource = { 0, 0, buttonTile.width, buttonTile.height };
					
						// Caso um personagem fique com a vida zerada ou negativa, ele some e sua Tile fica vazia (padrão).
						if(chimpanzini[r][c].hp <= 0 && chimpanzini[r][c].exists == true) {
							chimpanzini[r][c].exists = false;
							Tiles[r][c] = 1;
						}

						if(tralalero[r][c].hp <= 0 && tralalero[r][c].exists == true) {
							tralalero[r][c].exists = false;
							Tiles[r][c] = 1;
						}

						if(sahur[r][c].hp <= 0 && sahur[r][c].exists == true) {
							sahur[r][c].exists = false;
							Tiles[r][c] = 1;
						}

						if(lirili[r][c].hp <= 0 && lirili[r][c].exists == true) {
							lirili[r][c].exists = false;
							Tiles[r][c] = 1;
						}

						if(bombardini[r][c].hp <= 0 && bombardini[r][c].exists == true) {
							bombardini[r][c].exists = false;
							Tiles[r][c] = 1;
						}

                  		// Caso FrameCounterIdle seja divisível por determinado valor, incrementa o frame de cada personagem.
						if (FrameCounterIdle%8 == 0) {
							if(chimpanzini[r][c].exists == true) {
								chimpanzini[r][c].idle++;
							}

							if(tralalero[r][c].exists == true) {
								tralalero[r][c].idle++;
							}

							if(sahur[r][c].exists == true) {
								sahur[r][c].idle++;
							}

							if(bombardini[r][c].exists == true && bombardini[r][c].ready == false) {
								bombardini[r][c].idle++;
							}
						}
                
						// Lógica de comportamento dos personagens.
						if(chimpanzini[r][c].idle == 3  && chimpanzini[r][c].exists == true && chimpanzini[r][c].shining == false) {
							chimpanzini[r][c].idle = 0;
							chimpanzini[r][c].loop++;
						}

						if(chimpanzini[r][c].loop == 100) {
							chimpanzini[r][c].shining = true;
						}

						if(chimpanzini[r][c].idle == 7  && chimpanzini[r][c].exists == true && chimpanzini[r][c].shining == true) {
							chimpanzini[r][c].idle = 4;
							chimpanzini[r][c].loop = 0;
						}

						if(tralalero[r][c].idle == 3  && tralalero[r][c].exists == true && tralalero[r][c].attacking == false) {
							tralalero[r][c].idle = 0;
							tralalero[r][c].loop++;
						}

						if (tralalero[r][c].idle == 7 && tralalero[r][c].exists == true && tralalero[r][c].attacking == true) {
							tralalero[r][c].attacking = false;
							tralalero[r][c].projecB = true;
							tralalero[r][c].idle = 0;
						}

						if(tralalero[r][c].loop == 20 && tralalero[r][c].exists == true) {
							tralalero[r][c].attacking = true;
							tralalero[r][c].idle = 4;
							tralalero[r][c].loop = 0;
						}

						if(sahur[r][c].idle == 3  && sahur[r][c].exists == true) {
							sahur[r][c].idle = 0;
						}

						if(lirili[r][c].hp < 100 && lirili[r][c].hp >= 75 && lirili[r][c].exists == true) {
							lirili[r][c].idle = 1;
						}

						if(lirili[r][c].hp < 75 && lirili[r][c].hp >= 50 && lirili[r][c].exists == true) {
							lirili[r][c].idle = 2;
						}

						if(lirili[r][c].hp < 50 && lirili[r][c].exists == true) {
							lirili[r][c].idle = 3;
						}

						if(bombardini[r][c].idle == 3  && bombardini[r][c].exists == true && bombardini[r][c].ready == false) {
							bombardini[r][c].idle = 0;
							bombardini[r][c].loop++;
						}

						if(bombardini[r][c].loop == 300) {
							bombardini[r][c].ready = true;
							bombardini[r][c].idle = 4;
						}

						// Renderiza os personagens e Tiles, verificando seu código.
						switch (Tiles[r][c]) {
							case 0:
								DrawTexturePro(buttonTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								break;
							case 1:
								DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								break;
							case CHIMPANZINI:
								Rectangle chimpanziniDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 323/5, 543/5, screenWidth, screenHeight);
								DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								DrawTexturePro(CharacterTextures[0][chimpanzini[r][c].idle], chimpanziniSource, chimpanziniDest, Origin, 0.0f, WHITE);
								break;
							case TRALALERO:
								Rectangle tralaleroDest = ScaleRectTo720p(80 + (c * 96)-20, 220 + (r * 78), 186/2, 144/2, screenWidth, screenHeight);
								DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								DrawTexturePro(CharacterTextures[1][tralalero[r][c].idle], tralaleroSource, tralaleroDest, Origin, 0.0f, WHITE);
								break;
							case SAHUR:
								Rectangle sahurDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 122/2.5, 244/2.5, screenWidth, screenHeight);
								DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								DrawTexturePro(CharacterTextures[2][sahur[r][c].idle], sahurSource, sahurDest, Origin, 0.0f, WHITE);
								break;
							case LIRILI:
								Rectangle liriliDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 190/2.5, 225/2.5, screenWidth, screenHeight);
								DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								DrawTexturePro(CharacterTextures[3][lirili[r][c].idle], liriliSource, liriliDest, Origin, 0.0f, WHITE);
								break;
							case 20:
								Rectangle bombardiniDest = ScaleRectTo720p(80 + (c * 96)-2, 220 + (r * 78) + 9, 620/10, 610/10, screenWidth, screenHeight);
								DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								DrawTexturePro(CharacterTextures[4][bombardini[r][c].idle], bombardiniSource, bombardiniDest, Origin, 0.0f, WHITE);
								break;
							default:
								DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								break;
						}
                 
                
						if (Tiles[r][c] != 0 && CheckCollisionPointRec(mouse, tileDest)) {
							SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
							DrawRectangleRec(tileDest, ColorAlpha(YELLOW, 0.3f));

							if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
								// Lógica de Chimpanzini, quando ele estiver brilhando, recebe 25 pontos ("Dinheiro") e reseta o seu comportamento ao clicar nele.
								if (Tiles[r][c] == CHIMPANZINI && chimpanzini[r][c].shining == true) {
									chimpanzini[r][c].shining = false;
									Points += 25;
									chimpanzini[r][c].idle = 0;
								}
								
								// Lógica de posicionamento de personagem.
								if ( Tiles[r][c] == 1) {
									// Ao posicionar um personagem, ele é colocado e os valores de sua struct definidos.
									// Também verifica se o número de pontos ("Dinheiro") é maior que o custo do personagem, se não, não é possível posicioná-lo.
									if(MousePick >= CHIMPANZINI && MousePick <= 23 && Points >= CharacterCost[MousePick - CHIMPANZINI]) {
										switch(MousePick) {
											case CHIMPANZINI:
												chimpanzini[r][c].hp = 20;
												chimpanzini[r][c].idle = 0;
												chimpanzini[r][c].loop = 0;
												chimpanzini[r][c].shining = false;
												chimpanzini[r][c].exists = true;
												break;
											case TRALALERO:
												tralalero[r][c].hp = 50;
												tralalero[r][c].idle = 0;
												tralalero[r][c].loop = 0;
												tralalero[r][c].projecX = 80 + (c * 96)+35;
												tralalero[r][c].projecY = 220 + (r * 78);
												tralalero[r][c].projecB = false;
												tralalero[r][c].attacking = false;
												tralalero[r][c].exists = true;
												break;
											case SAHUR:
												sahur[r][c].hp = 50;
												sahur[r][c].idle = 0;
												sahur[r][c].wait = false;
												sahur[r][c].cooldown = 0;
												sahur[r][c].attacking = false;
												sahur[r][c].exists = true;
												break;
											case LIRILI:
												lirili[r][c].hp = 150;
												lirili[r][c].idle = 0;
												lirili[r][c].exists = true;
												break;
											case BOMBARDINI:
												bombardini[r][c].hp = 10;
												bombardini[r][c].idle = 0;
												bombardini[r][c].loop = 0;
												bombardini[r][c].bombX = 0;
												bombardini[r][c].bombY = 0;
												bombardini[r][c].bombB = false;
												bombardini[r][c].ready = false;
												bombardini[r][c].exists = true;
												break;
										}

										Tiles[r][c] = MousePick;
										Points -= CharacterCost[MousePick - CHIMPANZINI];
										MousePick = 1;
									}
								} else if(MousePick == 404) {
									// Lógica de venda de personagem, caso o usuário venda, o personagem some e metade do valor de custo é retornado ao usuário,
									// com exceção de Bombardini, que retorna 10 Pontos para o usuário.
									switch(Tiles[r][c]) {
										case CHIMPANZINI:
											chimpanzini[r][c].exists = false;
											break;
										case TRALALERO:
											tralalero[r][c].exists = false;
											break;
										case SAHUR:
											sahur[r][c].exists = false;
											break;
										case LIRILI:
											lirili[r][c].exists = false;
											break;
										case BOMBARDINI:
											bombardini[r][c].exists = false;
											Points -= 2;
											break;
									}

									Points += CharacterCost[Tiles[r][c]-CHIMPANZINI]/2;
									Tiles[r][c] = 1;
								}
							}
						}
					}
				}

				// Lógica de comportamento do projétil do Tralalero.
				for (int r = 0; r < ROWS; r++) {
					for (int c = 0; c < COLUMNS; c++) {
						if (tralalero[r][c].projecB == true) {
							tralalero[r][c].projecX += projectileSpeed * GetFrameTime();
							Rectangle projectileDest = ScaleRectTo720p((int)tralalero[r][c].projecX, tralalero[r][c].projecY + BOMBARDINI, 71, 29, screenWidth, screenHeight);
							DrawTexturePro(Projectile, projectileSource, projectileDest, Origin, 0.0f, WHITE);

							if (tralalero[r][c].projecX > screenWidth) {
								tralalero[r][c].projecX = 80 + (c * 96) + 35;
								tralalero[r][c].projecB = false;
							}
						}
					}
				}

				// Renderiza o personagem selecionado ao lado do mouse, de forma transparente.
				if(MousePick > 15 && MousePick < 24) { 
					Color Transparency = { 255, 255, 255, 128 };

					Rectangle texMSource = { 0, 0, CharacterFrames[2].width, CharacterFrames[2].height / 1.5f };

					Rectangle texMDest = ScaleRectTo720p( virtualMouseX + offsetX, virtualMouseY + offsetY, 78, 96, screenWidth, screenHeight);
					DrawTexturePro(CharacterFrames[MousePick-CHIMPANZINI], texMSource, texMDest, Origin, 0.0f, Transparency);
				}
        
        		// Lógica da bolsa de dinheiro aleatória.
				// Sempre que FrameCounterPisc for divisivel por um certo valor, altera o valor de piscBool.
				if(FrameCounterPisc%18==0) {
					piscBool = !piscBool;
				}

				// que altera o tamanho da bolsa, assim a fazendo piscar.
				if(piscBool == true) {
					pisc = 3;
				} else {
					pisc = 0;
				}

				// Caso o número aleatória seja 0, a bolsa aleatória aparece.
				if(RandomNum == 0 && !PointsBag) {
					PointsBag = true;
				}


				if(PointsBag) {
					Rectangle pointsBagSource = { 18, 11, 165, 210 };
					Rectangle pointsBagDest = ScaleRectTo720p(RandomNumX,RandomNumY, 78 + pisc, 96 + pisc, screenWidth, screenHeight);
					if(Randomize == true) {
						// Define FrameCounterPisc para 30 segundos.
						FrameCounterPisc = 3600/2;
					}

					// Para o Randomize, assim mantendo a bolsa no lugar e impedindo de gerar outra ao mesmo tempo.
					Randomize = false;

					// Decrementa FrameCounterPisc.
					FrameCounterPisc--;

					// A bolsa some caso não clicada em 30 segundos.
					if(FrameCounterPisc <= 0) {
						Randomize = true;
						PointsBag = false;
					}
            
					// Renderiza a bolsa.
					DrawTexturePro(pointsIcon, pointsBagSource, pointsBagDest, Origin, 0.0f, WHITE);

					if(CheckCollisionPointRec(mouse, pointsBagDest)) {
						SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
						
						// Se clicada, a bolsa dá ao usuário 25 pontos ("Dinheiro") e desaparece.
						if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
							Points += 25;
							PointsBag = false;
							FrameCounterPisc = -2;
							Randomize = true;
						}
					}
				}
        	}
        }

		EndDrawing();
	}

	CloseWindow();     

	return 0;
}
