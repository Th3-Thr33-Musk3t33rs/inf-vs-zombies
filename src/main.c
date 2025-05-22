#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "game.h"
#include "graphics.h"
#include "config.h"
#include "graphics.h"

int main(void) {

	InitGame();
	InitializeGameState(&game_state);

	// -------------------------------------------------------------------

	char PointsT[10], CostChar[10], RandomT[10];

    // Velocidade do projétil do Tralalero e da bomba do Bombardini.
    float projectileSpeed = 400.0f, bombSpeed = 1200.0f;

	int baseFontSize = 40;
	int fontSize = (int)(baseFontSize * ((float)BASE_HEIGHT_INT / 720.0f));

	while (!WindowShouldClose()) {
		// Detecta a posição do mouse.
        Vector2 mouse = GetMousePosition(), Origin = { 0, 0 };
		int virtualMouseX = (int)(mouse.x * 1280.0f / BASE_WIDTH_INT);
		int virtualMouseY = (int)(mouse.y * 720.0f / BASE_HEIGHT_INT);

        // Incrementa FrameCounterIdle no loop principal.
		game_state.frameCounterIdle++;

        // Reseta FrameCounterIdle após 1 minuto.
		if (game_state.frameCounterIdle == 3600) {
			game_state.frameCounterIdle = 0;
		}

		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		srand(time(NULL));

		// Variaveis para fazer a bolsa aparecer em locais e momentos aleatórios.
		int RandomNumY;
		int RandomNumX;
		int RandomNum;

		// Randomiza as varíaveis.
		if(game_state.randomize == true) {
			RandomNum = rand() % 20;
			RandomNumY = rand() % (BASE_HEIGHT_INT-50);
			RandomNumX = rand() % (BASE_WIDTH_INT-50);
		}


        // Pega as regiões certas das spritesheets.
		Rectangle sahurSource = { 8, 20, 122, 244 };
		Rectangle chimpanziniSource = { 32, 72, 323, 543 };
		Rectangle Source = { 32, 72, 323, 543 };
		Rectangle tralaleroSource = { 13, 57, 186, 144 };
		Rectangle projectileSource = { 5, 5, 71,29 };
		Rectangle liriliSource = { 35, 19, 190, 225 };
		Rectangle bombardiniSource = { 200, 205, 620, 610 };
		Rectangle pointsCountSource = { 275, 26, 179, 179 };

		BeginDrawing();
        ClearBackground(RAYWHITE);

        // Encerra o Jogo e vai pra tela de fim de jogo.
        if(!game_state.gameOver) {
			// Tela de Título antes do jogo começar.
			if(game_state.titleScreen) {
				// Título do Jogo e Botão de Iniciar Jogo.
				Rectangle playDest = ScaleRectTo720p( (int)1280/2.5-5, (int)720/2, 210, fontSize, BASE_WIDTH_INT, BASE_HEIGHT_INT);
				DrawText("Todos vs. Jacques", BASE_WIDTH_INT/3, BASE_HEIGHT_INT/3, fontSize, BLACK);
				DrawText("Play Game", BASE_WIDTH_INT/2.5, BASE_HEIGHT_INT/2, fontSize, BLACK);

				if (CheckCollisionPointRec(mouse, playDest)) {
					DrawRectangleRec(playDest, ColorAlpha(YELLOW, 0.3f));
					SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
					if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
						game_state.titleScreen = false;
					}
				}
			} else {
				// Variáveis para posicionar o personagem selecionado ao lado do mouse.
				int offsetX = 10;
				int offsetY = 10;

				// Mostrador de Número de pontos, o "Dinheiro".
				Vector2 textPoints = ScaleTo720p(110, 50, BASE_WIDTH_INT, BASE_HEIGHT_INT);
				float spacing = 10;

				sprintf(PointsT, "%d", game_state.points);

				sprintf(RandomT, "%d", RandomNum);

				int textWidth = MeasureText(PointsT, fontSize);
				float textHeight = fontSize;
				DrawText(RandomT, 100, 80, fontSize, BLACK);
				Rectangle pointsCountDest = {
					textPoints.x + textWidth + spacing,
					textPoints.y + (textHeight / 2.1f) - (35 * BASE_HEIGHT_INT / 720.0f) / 2.0f,
					35 * BASE_WIDTH_INT / 1280.0f,
					35 * BASE_HEIGHT_INT / 720.0f
				};

				// Texto do Número de pontos, o "Dinheiro".
				DrawText(PointsT, (int)textPoints.x, (int)textPoints.y, fontSize, BLACK);

				// Ícone de Moeda.
				DrawTexturePro(game_textures.pointsIcon, pointsCountSource, pointsCountDest, Origin, 0.0f, WHITE);


				// Botão/Texto de vender.
				Rectangle sellDest = ScaleRectTo720p(750-5, 40, 110, 50, BASE_WIDTH_INT, BASE_HEIGHT_INT);
				DrawText("SELL", sell_pos_x, sell_pos_y, fontSize, BLACK);

				if (CheckCollisionPointRec(mouse, sellDest)) {
					DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
					SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
						if(game_state.mousePick != SELL_ID) {
							game_state.mousePick = SELL_ID;
						} else {
							game_state.mousePick = 1;
						}
					}
				}

        		// Se Clicado, define como selecionado o valor 404 (o código de venda).
				if(game_state.mousePick == SELL_ID) {
					DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
				} 

				// For para inserir personagens no quadro de seleção.
				for (int f = 0; f < 5; f++) {
					Vector2 textValue = ScaleTo720p(310 + (f * 77), 117, BASE_WIDTH_INT, BASE_HEIGHT_INT);

					Rectangle frameDest = ScaleRectTo720p(300 + (f * 77), 20, 78, 96, BASE_WIDTH_INT, BASE_HEIGHT_INT);
					Rectangle frameSource = { 0, 0, game_textures.frame.width,  game_textures.frame.height };

					Rectangle charFrameDest = ScaleRectTo720p(300 + (f * 77), 29, 78, 82.75f, BASE_WIDTH_INT, BASE_HEIGHT_INT);
					Rectangle charFrameSource = { 0, 0, game_textures.characterFrames[2].width, game_textures.characterFrames[2].height / 1.5f };
					DrawTexturePro(game_textures.frame, frameSource, frameDest, Origin, 0.0f, WHITE);

					// Renderiza o custo dos personagens embaixo do quadro deles.
					sprintf(CostChar, "%d", game_state.characterCost[f]);
					DrawTexturePro(game_textures.characterFrames[f], charFrameSource, charFrameDest, Origin, 0.0f, WHITE);
					DrawText(CostChar, 12+(int)textValue.x, (int)textValue.y, fontSize/1.5, BLACK);


					// Define o código do personagem escolhido como selecionado.
					if (CheckCollisionPointRec(mouse, frameDest)) {
						DrawRectangleRec(frameDest, ColorAlpha(YELLOW, 0.3f));
						SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
						if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
							game_state.mousePick = game_state.frame[f];
						}
					}
				}

				// For Principal.
				for (int r = 0; r < ROWS; r++) {
					for (int c = 0; c < COLUMNS; c++) {
						// Define Tamanho e região do spritesheet das Tiles.
						Rectangle tileDest = ScaleRectTo720p(60 + (c * 96), 220 + (r * 78), 96, 78, BASE_WIDTH_INT, BASE_HEIGHT_INT);
						Rectangle tileSource = { 0, 0, game_textures.buttonTile.width, game_textures.buttonTile.height };

						// Caso um personagem fique com a vida zerada ou negativa, ele some e sua Tile fica vazia (padrão).
						if(game_state.chimpanzini[r][c].hp <= 0 && game_state.chimpanzini[r][c].exists == true) {
							game_state.chimpanzini[r][c].exists = false;
							game_state.tiles[r][c] = 1;
						}

						if(game_state.tralalero[r][c].hp <= 0 && game_state.tralalero[r][c].exists == true) {
							game_state.tralalero[r][c].exists = false;
							game_state.tiles[r][c] = 1;
						}

						if(game_state.sahur[r][c].hp <= 0 && game_state.sahur[r][c].exists == true) {
							game_state.sahur[r][c].exists = false;
							game_state.tiles[r][c] = 1;
						}

						if(game_state.lirili[r][c].hp <= 0 && game_state.lirili[r][c].exists == true) {
							game_state.lirili[r][c].exists = false;
							game_state.tiles[r][c] = 1;
						}

						if(game_state.bombardini[r][c].hp <= 0 && game_state.bombardini[r][c].exists == true) {
							game_state.bombardini[r][c].exists = false;
							game_state.tiles[r][c] = 1;
						}

                  		// Caso FrameCounterIdle seja divisível por determinado valor, incrementa o frame de cada personagem.
						if (game_state.frameCounterIdle%8 == 0) {
							if(game_state.chimpanzini[r][c].exists == true) {
								game_state.chimpanzini[r][c].idle++;
							}

							if(game_state.tralalero[r][c].exists == true) {
								game_state.tralalero[r][c].idle++;
							}

							if(game_state.sahur[r][c].exists == true) {
								game_state.sahur[r][c].idle++;
							}

							if(game_state.bombardini[r][c].exists == true && game_state.bombardini[r][c].ready == false) {
								game_state.bombardini[r][c].idle++;
							}
						}

						// Lógica de comportamento dos personagens.
						if(game_state.chimpanzini[r][c].idle == 3  && game_state.chimpanzini[r][c].exists == true && game_state.chimpanzini[r][c].shining == false) {
							game_state.chimpanzini[r][c].idle = 0;
							game_state.chimpanzini[r][c].loop++;
						}

						if(game_state.chimpanzini[r][c].loop == 100) {
							game_state.chimpanzini[r][c].shining = true;
						}

						if(game_state.chimpanzini[r][c].idle == 7  && game_state.chimpanzini[r][c].exists == true && game_state.chimpanzini[r][c].shining == true) {
							game_state.chimpanzini[r][c].idle = 4;
							game_state.chimpanzini[r][c].loop = 0;
						}

						if(game_state.tralalero[r][c].idle == 3  && game_state.tralalero[r][c].exists == true && game_state.tralalero[r][c].attacking == false) {
							game_state.tralalero[r][c].idle = 0;
							game_state.tralalero[r][c].loop++;
						}

						if (game_state.tralalero[r][c].idle == 7 && game_state.tralalero[r][c].exists == true && game_state.tralalero[r][c].attacking == true) {
							game_state.tralalero[r][c].attacking = false;
							game_state.tralalero[r][c].projecB = true;
							game_state.tralalero[r][c].idle = 0;
						}

						if(game_state.tralalero[r][c].loop == 20 && game_state.tralalero[r][c].exists == true) {
							game_state.tralalero[r][c].attacking = true;
							game_state.tralalero[r][c].idle = 4;
							game_state.tralalero[r][c].loop = 0;
						}

						if(game_state.sahur[r][c].idle == 3  && game_state.sahur[r][c].exists == true) {
							game_state.sahur[r][c].idle = 0;
						}

						if(game_state.lirili[r][c].hp < 100 && game_state.lirili[r][c].hp >= 75 && game_state.lirili[r][c].exists == true) {
							game_state.lirili[r][c].idle = 1;
						}

						if(game_state.lirili[r][c].hp < 75 && game_state.lirili[r][c].hp >= 50 && game_state.lirili[r][c].exists == true) {
							game_state.lirili[r][c].idle = 2;
						}

						if(game_state.lirili[r][c].hp < 50 && game_state.lirili[r][c].exists == true) {
							game_state.lirili[r][c].idle = 3;
						}

						if(game_state.bombardini[r][c].idle == 3  && game_state.bombardini[r][c].exists == true && game_state.bombardini[r][c].ready == false) {
							game_state.bombardini[r][c].idle = 0;
							game_state.bombardini[r][c].loop++;
						}

						if(game_state.bombardini[r][c].loop == 300) {
							game_state.bombardini[r][c].ready = true;
							game_state.bombardini[r][c].idle = 4;
						}

						// Renderiza os personagens e Tiles, verificando seu código.
						switch (game_state.tiles[r][c]) {
							case 0:
								DrawTexturePro(game_textures.buttonTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								break;
							case 1:
								DrawTexturePro(game_textures.metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								break;
							case CHIMPANZINI_ID:
								Rectangle chimpanziniDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 323/5, 543/5, BASE_WIDTH_INT, BASE_HEIGHT_INT);
								DrawTexturePro(game_textures.metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								DrawTexturePro(game_textures.characterTextures[0][game_state.chimpanzini[r][c].idle], chimpanziniSource, chimpanziniDest, Origin, 0.0f, WHITE);
								break;
							case TRALALERO_ID:
								Rectangle tralaleroDest = ScaleRectTo720p(80 + (c * 96)-20, 220 + (r * 78), 186/2, 144/2, BASE_WIDTH_INT, BASE_HEIGHT_INT);
								DrawTexturePro(game_textures.metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								DrawTexturePro(game_textures.characterTextures[1][game_state.tralalero[r][c].idle], tralaleroSource, tralaleroDest, Origin, 0.0f, WHITE);
								break;
							case SAHUR_ID:
								Rectangle sahurDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 122/2.5, 244/2.5, BASE_WIDTH_INT, BASE_HEIGHT_INT);
								DrawTexturePro(game_textures.metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								DrawTexturePro(game_textures.characterTextures[2][game_state.sahur[r][c].idle], sahurSource, sahurDest, Origin, 0.0f, WHITE);
								break;
							case LIRILI_ID:
								Rectangle liriliDest = ScaleRectTo720p(80 + (c * 96), 220 + (r * 78) - 10, 190/2.5, 225/2.5, BASE_WIDTH_INT, BASE_HEIGHT_INT);
								DrawTexturePro(game_textures.metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								DrawTexturePro(game_textures.characterTextures[3][game_state.lirili[r][c].idle], liriliSource, liriliDest, Origin, 0.0f, WHITE);
								break;
							case 20:
								Rectangle bombardiniDest = ScaleRectTo720p(80 + (c * 96)-2, 220 + (r * 78) + 9, 620/10, 610/10, BASE_WIDTH_INT, BASE_HEIGHT_INT);
								DrawTexturePro(game_textures.metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								DrawTexturePro(game_textures.characterTextures[4][game_state.bombardini[r][c].idle], bombardiniSource, bombardiniDest, Origin, 0.0f, WHITE);
								break;
							default:
								DrawTexturePro(game_textures.metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
								break;
						}


						if (game_state.tiles[r][c] != 0 && CheckCollisionPointRec(mouse, tileDest)) {
							SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
							DrawRectangleRec(tileDest, ColorAlpha(YELLOW, 0.3f));

							if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
								// Lógica de Chimpanzini, quando ele estiver brilhando, recebe 25 pontos ("Dinheiro") e reseta o seu comportamento ao clicar nele.
								if (game_state.tiles[r][c] == CHIMPANZINI_ID && game_state.chimpanzini[r][c].shining == true) {
									game_state.chimpanzini[r][c].shining = false;
									game_state.points += 25;
									game_state.chimpanzini[r][c].idle = 0;
								}

								// Lógica de posicionamento de personagem.
								if ( game_state.tiles[r][c] == 1) {
									// Ao posicionar um personagem, ele é colocado e os valores de sua struct definidos.
									// Também verifica se o número de pontos ("Dinheiro") é maior que o custo do personagem, se não, não é possível posicioná-lo.
									if(game_state.mousePick >= CHIMPANZINI_ID && game_state.mousePick <= 23 && game_state.points >= game_state.characterCost[game_state.mousePick - CHIMPANZINI_ID]) {
										switch(game_state.mousePick) {
											case CHIMPANZINI_ID:
												game_state.chimpanzini[r][c].hp = 20;
												game_state.chimpanzini[r][c].idle = 0;
												game_state.chimpanzini[r][c].loop = 0;
												game_state.chimpanzini[r][c].shining = false;
												game_state.chimpanzini[r][c].exists = true;
												break;
											case TRALALERO_ID:
												game_state.tralalero[r][c].hp = 50;
												game_state.tralalero[r][c].idle = 0;
												game_state.tralalero[r][c].loop = 0;
												game_state.tralalero[r][c].projecX = 80 + (c * 96)+35;
												game_state.tralalero[r][c].projecY = 220 + (r * 78);
												game_state.tralalero[r][c].projecB = false;
												game_state.tralalero[r][c].attacking = false;
												game_state.tralalero[r][c].exists = true;
												break;
											case SAHUR_ID:
												game_state.sahur[r][c].hp = 50;
												game_state.sahur[r][c].idle = 0;
												game_state.sahur[r][c].wait = false;
												game_state.sahur[r][c].cooldown = 0;
												game_state.sahur[r][c].attacking = false;
												game_state.sahur[r][c].exists = true;
												break;
											case LIRILI_ID:
												game_state.lirili[r][c].hp = 150;
												game_state.lirili[r][c].idle = 0;
												game_state.lirili[r][c].exists = true;
												break;
											case BOMBARDINI_ID:
												game_state.bombardini[r][c].hp = 10;
												game_state.bombardini[r][c].idle = 0;
												game_state.bombardini[r][c].loop = 0;
												game_state.bombardini[r][c].bombX = 0;
												game_state.bombardini[r][c].bombY = 0;
												game_state.bombardini[r][c].bombB = false;
												game_state.bombardini[r][c].ready = false;
												game_state.bombardini[r][c].exists = true;
												break;
										}

										game_state.tiles[r][c] = game_state.mousePick;
										game_state.points -= game_state.characterCost[game_state.mousePick - CHIMPANZINI_ID];
										game_state.mousePick = 1;
									}
								} else if(game_state.mousePick == SELL_ID) {
									// Lógica de venda de personagem, caso o usuário venda, o personagem some e metade do valor de custo é retornado ao usuário,
									// com exceção de Bombardini, que retorna 10 Pontos para o usuário.
									switch(game_state.tiles[r][c]) {
										case CHIMPANZINI_ID:
											game_state.chimpanzini[r][c].exists = false;
											break;
										case TRALALERO_ID:
											game_state.tralalero[r][c].exists = false;
											break;
										case SAHUR_ID:
											game_state.sahur[r][c].exists = false;
											break;
										case LIRILI_ID:
											game_state.lirili[r][c].exists = false;
											break;
										case BOMBARDINI_ID:
											game_state.bombardini[r][c].exists = false;
											game_state.points -= 2;
											break;
									}

									game_state.points += game_state.characterCost[game_state.tiles[r][c]-CHIMPANZINI_ID]/2;
									game_state.tiles[r][c] = 1;
								}
							}
						}
					}
				}

				// Lógica de comportamento do projétil do Tralalero.
				for (int r = 0; r < ROWS; r++) {
					for (int c = 0; c < COLUMNS; c++) {
						if (game_state.tralalero[r][c].projecB == true) {
							game_state.tralalero[r][c].projecX += projectileSpeed * GetFrameTime();
							Rectangle projectileDest = ScaleRectTo720p((int)game_state.tralalero[r][c].projecX, game_state.tralalero[r][c].projecY + BOMBARDINI_ID, 71, 29, BASE_WIDTH_INT, BASE_HEIGHT_INT);
							DrawTexturePro(game_textures.projectile, projectileSource, projectileDest, Origin, 0.0f, WHITE);

							if (game_state.tralalero[r][c].projecX > BASE_WIDTH_INT) {
								game_state.tralalero[r][c].projecX = 80 + (c * 96) + 35;
								game_state.tralalero[r][c].projecB = false;
							}
						}
					}
				}

				// Renderiza o personagem selecionado ao lado do mouse, de forma transparente.
				if(game_state.mousePick > 15 && game_state.mousePick < 24) {
					Color Transparency = { 255, 255, 255, 128 };

					Rectangle texMSource = { 0, 0, game_textures.characterFrames[2].width, game_textures.characterFrames[2].height / 1.5f };

					Rectangle texMDest = ScaleRectTo720p( virtualMouseX + offsetX, virtualMouseY + offsetY, 78, 96, BASE_WIDTH_INT, BASE_HEIGHT_INT);
					DrawTexturePro(game_textures.characterFrames[game_state.mousePick-CHIMPANZINI_ID], texMSource, texMDest, Origin, 0.0f, Transparency);
				}

        		// Lógica da bolsa de dinheiro aleatória.
				// Sempre que FrameCounterPisc for divisivel por um certo valor, altera o valor de piscBool.
				if(game_state.frameCounterPisc%18==0) {
					game_state.piscBool = !game_state.piscBool;
				}

				// que altera o tamanho da bolsa, assim a fazendo piscar.
				if(game_state.piscBool == true) {
					game_state.pisc = 3;
				} else {
					game_state.pisc = 0;
				}

				// Caso o número aleatória seja 0, a bolsa aleatória aparece.
				if(RandomNum == 0 && !game_state.pointsBag) {
					game_state.pointsBag = true;
				}


				if(game_state.pointsBag) {
					Rectangle pointsBagSource = { 18, 11, 165, 210 };
					Rectangle pointsBagDest = ScaleRectTo720p(RandomNumX,RandomNumY, 78 + game_state.pisc, 96 + game_state.pisc, BASE_WIDTH_INT, BASE_HEIGHT_INT);
					if(game_state.randomize == true) {
						// Define FrameCounterPisc para 30 segundos.
						game_state.frameCounterPisc = 3600/2;
					}

					// Para o Randomize, assim mantendo a bolsa no lugar e impedindo de gerar outra ao mesmo tempo.
					game_state.randomize = false;

					// Decrementa FrameCounterPisc.
					game_state.frameCounterPisc--;

					// A bolsa some caso não clicada em 30 segundos.
					if(game_state.frameCounterPisc <= 0) {
						game_state.randomize = true;
						game_state.pointsBag = false;
					}

					// Renderiza a bolsa.
					DrawTexturePro(game_textures.pointsIcon, pointsBagSource, pointsBagDest, Origin, 0.0f, WHITE);

					if(CheckCollisionPointRec(mouse, pointsBagDest)) {
						SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

						// Se clicada, a bolsa dá ao usuário 25 pontos ("Dinheiro") e desaparece.
						if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
							game_state.points += 25;
							game_state.pointsBag = false;
							game_state.frameCounterPisc = -2;
							game_state.randomize = true;
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
