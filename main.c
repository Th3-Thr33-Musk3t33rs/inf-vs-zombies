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

Vector2 ScaleTo720p(float x, float y, int screenWidth, int screenHeight) {
	float baseWidth = 1280.0f;
	float baseHeight = 720.0f;
	return (Vector2) {
		x * ((float)screenWidth / baseWidth), y * ((float)screenHeight / baseHeight)
	};
}

Rectangle ScaleRectTo720p(float x, float y, float width, float height, int screenWidth, int screenHeight) {
	Vector2 pos = ScaleTo720p(x, y, screenWidth, screenHeight);
	Vector2 size = ScaleTo720p(width, height, screenWidth, screenHeight);
	return (Rectangle) {
		pos.x, pos.y, size.x, size.y
	};
}



//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 1280;
	const int screenHeight = 720;
	float scaleX = (float)screenWidth / BASE_WIDTH;
	float scaleY = (float)screenHeight / BASE_HEIGHT;

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	//--------------------------------------------------------------------------------------

	Texture2D metallicTile, buttonTile, frame, CharacterTextures[8][8], CharacterFrames[8], pointsIcon, Projectile;
	metallicTile = LoadTexture("metallictile.png");
	buttonTile = LoadTexture("buttontilemetallic.png");
	frame = LoadTexture("frame2.png");
	pointsIcon = LoadTexture("Points.png");
	const char* nomes[8] = {
		"chimpanzini", "tralalero", "sahur", "lirili",
		"bombardini", "trippitroppa", "capuccino", "patapim"
	};

	char path[100];
	for(int t = 0; t < 8; t++) {
		for (int i = 0; i < 8; i++) {
			sprintf(path, "Characters/%s%d.png", nomes[i], t);
			CharacterTextures[i][t] = LoadTexture(path);
		}
	}
	CharacterFrames[0] = LoadTexture("Characters/chimpanziniframe.png");
	CharacterFrames[1] = LoadTexture("Characters/tralaleroframe.png");
	CharacterFrames[2] = LoadTexture("Characters/sahurframe.png");
	CharacterFrames[3] = LoadTexture("Characters/liriliframe.png");
	CharacterFrames[4] = LoadTexture("Characters/bombardiniframe.png");
	CharacterFrames[5] = LoadTexture("Characters/trippitroppaframe.png");
	CharacterFrames[6] = LoadTexture("Characters/capuccinoframe.png");
	CharacterFrames[7] = LoadTexture("Characters/patapimframe.png");

    Projectile = LoadTexture("Characters/projectile.png");



	int Tiles[7][9], Frame[8], CharacterCost[8];
	CharacterCost[0] = 50;
	CharacterCost[1] = 100;
	CharacterCost[2] = 150;
	CharacterCost[3] = 50;
	CharacterCost[4] = 50;
	CharacterCost[5] = 50;
	CharacterCost[6] = 50;
	CharacterCost[7] = 50;
	int MousePick = 1, Points = 150, Damage = 10, CharacterHP = 50,
	    EnemieHP = 110;
	char PointsT[10], CostChar[10], RandomT[10];

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
	Chimpanzini chimpanzini[7][9] = { 0, 0, 0, false, false };
	Tralalero tralalero[7][9] = { 0, 0, 0, 0, 0, false, false, false };
	Sahur sahur[7][9] = { 0, 0, false, 0,false, false };
    Lirili lirili[7][9] = { 0, 0, false,};

	int FrameCounterPisc = 0, pisc = 0,FrameCounterIdle = 0;
	bool PointsBag = false, Randomize = true, piscBool = true;
	for(int r = 0; r < 7; r++)
	{
		for(int c = 0; c < 9; c++)
		{
			if(c == 0)
			{

				Tiles[r][c] = 0;
			} else {

				Tiles[r][c] = 1;
			}
		
		
          
		}
	}
	for(int f = 16; f < 24; f++) {
		Frame[f-16] = f;

	}
	
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		FrameCounterIdle++;
		if (FrameCounterIdle == 3600) {
			FrameCounterIdle = 0;
		}


		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		srand(time(NULL));
		int RandomNumY;
		int RandomNumX;
		int RandomNum;
		if(Randomize == true) {
			RandomNum = rand() % 20;
			RandomNumY = rand() % (screenHeight-50);
			RandomNumX = rand() % (screenWidth-50);
		}
        
        

		Rectangle sahurSource = { 8, 20, 122, 244 };
		Rectangle chimpanziniSource = { 32, 72, 323, 543 };
		Rectangle tralaleroSource = { 13, 57, 186, 144 };
        Rectangle projectileSource = { 5, 5, 71,29 };
        Rectangle liriliSource = { 35, 19, 190, 225 };
       
		Rectangle pointsCountSource = { 275, 26, 179, 179 };
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		Vector2 mouse = GetMousePosition(), Origin = { 0, 0 };
		int offsetX = 10;
		int offsetY = 10;
		Vector2 textPoints = ScaleTo720p(110, 50, screenWidth, screenHeight);
		float spacing = 10;
        float projectileSpeed = 400.0f;
		int baseFontSize = 40;
		int fontSize = (int)(baseFontSize * ((float)screenHeight / 720.0f));

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

		DrawText(PointsT, (int)textPoints.x, (int)textPoints.y, fontSize, BLACK);
		Rectangle sellDest = ScaleRectTo720p((int)textPoints.x*9-5, (int)textPoints.y-5, 110, 50, screenWidth, screenHeight);
		DrawText("SELL", (int)textPoints.x*9, (int)textPoints.y, fontSize, BLACK);

		if (CheckCollisionPointRec(mouse, sellDest))
		{


			DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));
			SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				if(MousePick != 404) {
					MousePick = 404;
				} else {
					MousePick = 1;
				}
			}
		}
		if(MousePick == 404) {
			DrawRectangleRec(sellDest, ColorAlpha(YELLOW, 0.3f));

		}

		DrawTexturePro(pointsIcon, pointsCountSource, pointsCountDest, Origin, 0.0f, WHITE);
		ClearBackground(RAYWHITE);
		for (int f = 0; f < 8; f++) {
			Vector2 textValue = ScaleTo720p(310 + (f * 77), 117, screenWidth, screenHeight);

			Rectangle frameDest = ScaleRectTo720p(300 + (f * 77), 20, 78, 96, screenWidth, screenHeight);
			Rectangle frameSource = { 0, 0, frame.width, frame.height };

			Rectangle charFrameDest = ScaleRectTo720p(300 + (f * 77), 28, 78, 82.75f, screenWidth, screenHeight);
			Rectangle charFrameSource = { 0, 0, CharacterFrames[2].width, CharacterFrames[2].height / 1.5f };
			DrawTexturePro(frame, frameSource, frameDest, Origin, 0.0f, WHITE);


			sprintf(CostChar, "%d", CharacterCost[f]);
			DrawTexturePro(CharacterFrames[f], charFrameSource, charFrameDest, Origin, 0.0f, WHITE);
			DrawText(CostChar, 12+(int)textValue.x, (int)textValue.y, fontSize/1.5, BLACK);



			if (CheckCollisionPointRec(mouse, frameDest))
			{


				DrawRectangleRec(frameDest, ColorAlpha(YELLOW, 0.3f));
				SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					MousePick = Frame[f];
				}

			}
		}


		for (int r = 0; r < 7; r++)
		{
			for (int c = 0; c < 9; c++)
			{
				Rectangle tileDest = ScaleRectTo720p(60 + (c * 96), 160 + (r * 78), 96, 78, screenWidth, screenHeight);
				Rectangle tileSource = { 0, 0, buttonTile.width, buttonTile.height };
                
                if(chimpanzini[r][c].hp <= 0 && chimpanzini[r][c].exists == true){
                    chimpanzini[r][c].exists = false;
                    Tiles[r][c] = 1;
                    
                }
                  if(tralalero[r][c].hp <= 0 && tralalero[r][c].exists == true){
                    tralalero[r][c].exists = false;
                    Tiles[r][c] = 1;
                    
                }
                  if(sahur[r][c].hp <= 0 && sahur[r][c].exists == true){
                    sahur[r][c].exists = false;
                    Tiles[r][c] = 1;
                    
                }
                  if(lirili[r][c].hp <= 0 && lirili[r][c].exists == true){
                    lirili[r][c].exists = false;
                    Tiles[r][c] = 1;
                    
                }



				if (FrameCounterIdle%8 == 0  && chimpanzini[r][c].exists == true) {
					chimpanzini[r][c].idle++;
				}
				if(chimpanzini[r][c].idle == 3  && chimpanzini[r][c].exists == true && chimpanzini[r][c].shining == false) {
					chimpanzini[r][c].idle = 0;
                    chimpanzini[r][c].loop++;
				}
                if(chimpanzini[r][c].loop == 100){
                    chimpanzini[r][c].shining = true;
                }
                if(chimpanzini[r][c].idle == 7  && chimpanzini[r][c].exists == true && chimpanzini[r][c].shining == true) {
					chimpanzini[r][c].idle = 4;
                     chimpanzini[r][c].loop = 0;
				}
                if (FrameCounterIdle%8 == 0  && tralalero[r][c].exists == true) {
					tralalero[r][c].idle++;
				}
				if(tralalero[r][c].idle == 3  && tralalero[r][c].exists == true && tralalero[r][c].attacking == false) {
					tralalero[r][c].idle = 0;
                    tralalero[r][c].loop++;
				}
                if (tralalero[r][c].idle == 7 && tralalero[r][c].exists == true && tralalero[r][c].attacking == true){
                     tralalero[r][c].attacking = false;
                    tralalero[r][c].projecB = true;
                    tralalero[r][c].idle = 0;
                }
               
                if(tralalero[r][c].loop == 20 && tralalero[r][c].exists == true) {
                  tralalero[r][c].attacking = true;
                  tralalero[r][c].idle = 4;
                  tralalero[r][c].loop = 0;
                }
                if (FrameCounterIdle%8 == 0  && sahur[r][c].exists == true) {
					sahur[r][c].idle++;
				}
				if(sahur[r][c].idle == 3  && sahur[r][c].exists == true) {
					sahur[r][c].idle = 0;
				}
                if(lirili[r][c].hp < 100 && lirili[r][c].hp >= 75 && lirili[r][c].exists == true){
                    lirili[r][c].idle = 1;
                }
                 if(lirili[r][c].hp < 75 && lirili[r][c].hp >= 50 && lirili[r][c].exists == true){
                    lirili[r][c].idle = 2;
                }
                 if(lirili[r][c].hp < 50 && lirili[r][c].exists == true){
                    lirili[r][c].idle = 3;
                }
                 
				switch (Tiles[r][c])
				{
				case 0:
					DrawTexturePro(buttonTile, tileSource, tileDest, Origin, 0.0f, WHITE);
					break;
				case 1:
					DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
					break;
				case 16:
					Rectangle chimpanziniDest = ScaleRectTo720p(80 + (c * 96), 160 + (r * 78) - 10, 323/5, 543/5, screenWidth, screenHeight);
					DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
					DrawTexturePro(CharacterTextures[0][chimpanzini[r][c].idle], chimpanziniSource, chimpanziniDest, Origin, 0.0f, WHITE);
					break;
				case 17:
					Rectangle tralaleroDest = ScaleRectTo720p(80 + (c * 96)-20, 160 + (r * 78), 186/2, 144/2, screenWidth, screenHeight);
					DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
					DrawTexturePro(CharacterTextures[1][tralalero[r][c].idle], tralaleroSource, tralaleroDest, Origin, 0.0f, WHITE);
					break;
				case 18: {
					Rectangle sahurDest = ScaleRectTo720p(80 + (c * 96), 160 + (r * 78) - 10, 122/2.5, 244/2.5, screenWidth, screenHeight);
					DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
					DrawTexturePro(CharacterTextures[2][sahur[r][c].idle], sahurSource, sahurDest, Origin, 0.0f, WHITE);
					break;
				}
                	case 19: {
					Rectangle liriliDest = ScaleRectTo720p(80 + (c * 96), 160 + (r * 78) - 10, 190/2.5, 225/2.5, screenWidth, screenHeight);
					DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
					DrawTexturePro(CharacterTextures[3][lirili[r][c].idle], liriliSource, liriliDest, Origin, 0.0f, WHITE);
					break;
				}
				default:
					DrawTexturePro(metallicTile, tileSource, tileDest, Origin, 0.0f, WHITE);
					break;
				}

				if (Tiles[r][c] != 0 && CheckCollisionPointRec(mouse, tileDest))
				{
					SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

					DrawRectangleRec(tileDest, ColorAlpha(YELLOW, 0.3f));
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{

                        if (Tiles[r][c] == 16 && chimpanzini[r][c].shining == true){
                            chimpanzini[r][c].shining = false;
                            Points += 25;
                            	chimpanzini[r][c].idle = 0;
                        }
						if ( Tiles[r][c] == 1) {
							if(Points >= CharacterCost[MousePick-16])
							{

								Tiles[r][c] = MousePick;
								switch(MousePick) {
									{

									case 16:
										chimpanzini[r][c].hp = 20;
                                        chimpanzini[r][c].idle = 0;
										chimpanzini[r][c].loop = 0;
										chimpanzini[r][c].shining = false;
										chimpanzini[r][c].exists = true;
										break;
									case 17:
										tralalero[r][c].hp = 50;
                                        tralalero[r][c].idle = 0;
										tralalero[r][c].loop = 0;
                                        tralalero[r][c].projecX = 80 + (c * 96)+35;
                                        tralalero[r][c].projecY = 160 + (r * 78);
                                        tralalero[r][c].projecB = false;
                                        tralalero[r][c].attacking = false;
										tralalero[r][c].exists = true;
										break;
									case 18:
										sahur[r][c].hp = 50;
                                        sahur[r][c].idle = 0;
										sahur[r][c].wait = false;
										sahur[r][c].cooldown = 0;
                                        sahur[r][c].attacking = false;
										sahur[r][c].exists = true;
										break;
									}
                                    case 19:
										lirili[r][c].hp = 150;
                                        lirili[r][c].idle = 0;
										lirili[r][c].exists = true;
										break;
									}
								

								Points -= CharacterCost[MousePick-16];

								MousePick = 1;
                        }
                        }else if(MousePick == 404)
							{
                                
                                	switch(Tiles[r][c]) {
									{

									case 16:
										chimpanzini[r][c].exists = false;
										break;
									case 17:
										tralalero[r][c].exists = false;
										break;
									case 18:
										sahur[r][c].exists = false;
										break;
									}
                                    case 19:  
										lirili[r][c].exists = false;
										break;
									}
								Points += CharacterCost[Tiles[r][c]-16]/2;
								Tiles[r][c] = 1;


							}
						} 

					}
                   
                
				}
                
			}
		
		for (int r = 0; r < 7; r++)
{
    for (int c = 0; c < 9; c++)
    {
        if (tralalero[r][c].projecB == true)
        {
            tralalero[r][c].projecX += projectileSpeed * GetFrameTime();
            Rectangle projectileDest = ScaleRectTo720p((int)tralalero[r][c].projecX, tralalero[r][c].projecY + 20, 71, 29, screenWidth, screenHeight);
            DrawTexturePro(Projectile, projectileSource, projectileDest, Origin, 0.0f, WHITE);

            if (tralalero[r][c].projecX > screenWidth)
            {
                tralalero[r][c].projecX = 80 + (c * 96) + 35;
                tralalero[r][c].projecB = false;
            }
        }
    }
}

		if(MousePick > 15 && MousePick < 24) {
			Color Transparency = { 255, 255, 255, 128 };

			Rectangle texMSource = { 0, 0, CharacterFrames[2].width, CharacterFrames[2].height / 1.5f };
			Rectangle texMDest = ScaleRectTo720p(mouse.x + offsetX, mouse.y + offsetY, 78, 96, screenWidth, screenHeight);

			DrawTexturePro(CharacterFrames[MousePick-16], texMSource, texMDest, Origin, 0.0f, Transparency);
		}
		if(FrameCounterPisc%18==0) {
			piscBool = !piscBool;
		}
		if(piscBool == true) {
			pisc = 3;
		} else {
			pisc = 0;
		}
		if(RandomNum == 0) {

			PointsBag = true;

		}


		if(PointsBag)
		{

			Rectangle pointsBagSource = { 18, 11, 165, 210 };
			Rectangle pointsBagDest = ScaleRectTo720p(RandomNumX,RandomNumY, 78 + pisc, 96 + pisc, screenWidth, screenHeight);
			if(Randomize == true) {

				FrameCounterPisc = (60*60)/2;

			}
			Randomize = false;
			FrameCounterPisc--;
			if(FrameCounterPisc <= 0)
			{
				Randomize = true;
				PointsBag = false;
			}
			DrawTexturePro(pointsIcon, pointsBagSource, pointsBagDest, Origin, 0.0f, WHITE);

			if(CheckCollisionPointRec(mouse, pointsBagDest)) {
				SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

				if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
                     Points += 25;
        PointsBag = false;  
        FrameCounterPisc = 0;
        RandomNum = 6;
        Randomize = true;


				}

			}


		}








		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------

	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}