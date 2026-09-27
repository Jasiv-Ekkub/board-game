#include <board.h>
#include <assets.h>
#include <game_context.h>
#include <raygui.h>
#include <layout_engine.h>
#include <gui_elements.h>
#include <stdbool.h>
#include <stdio.h>
#include <animator.h>

#define GAME_NAME "Buissnessland"

Color backgroundColor;
Color textColorNormal;
Color textColorFocused;

enum { START_SCREEN, MAIN_MENU, HOW_TO_PLAY, BOARD, EXIT_SCREEN } phase = START_SCREEN;
static const bool showOptionsButton[] = {
	[START_SCREEN] = false,
	[MAIN_MENU] = true,
	[BOARD] = true,
	[HOW_TO_PLAY] = false,
	[EXIT_SCREEN] = false,
};
bool shouldClose = false;
Board board;

Timer timer;

bool showOptions = false;

void HandleInitialScreen();
void HandleMainMenu();
void HandleHowToPlay();
void HandleBoard();
void HandleExitScreen();

int main()
{
	InitializeGameContext();
	LoadAssets();
	
	timer = Timer_Get();

	board = GetBoard();

	backgroundColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
	textColorNormal = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
	textColorFocused = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_FOCUSED));
	while(!shouldClose)
	{
		shouldClose = WindowShouldClose();
		UpdateGameContext();
		UpdateAnimator();
		Timer_Update(&timer);

		BeginDrawing();
			ClearBackground(backgroundColor);
			if(showOptions) GuiLock();
			switch(phase)
			{
				case START_SCREEN:
					HandleInitialScreen();
					break;
				case MAIN_MENU:
					HandleMainMenu();
					break;
				case HOW_TO_PLAY:
					HandleHowToPlay();
					break;
				case BOARD:
					HandleBoard();
					break;
				case EXIT_SCREEN:
					HandleExitScreen();
					break;
			}
			if(showOptionsButton[phase])
			{
				if(showOptions)
				{
					GuiUnlock();
					GuiPanel(GetRectanglePlacement(0, 80, 800, 500, CENTER, CENTER), 0);
					
					static int volume = 10;
					if(GuiSpinnerSfx(GetRectanglePlacement(60, -80, 480, 70, CENTER, CENTER), "Volume ", &volume, 0, 10))
					{
						SetMasterVolume((float)volume/10);
					}

					static int zoom = 0;
					if(GuiSpinnerSfx(GetRectanglePlacement(60, 0, 480, 70, CENTER, CENTER), "Zoom ", &zoom, 0, 10))
					{
						SetCameraFov((float)(-zoom) / 20 + 2);
					}

					static int guiScale = 5;
					if(GuiSpinnerSfx(GetRectanglePlacement(60, 80, 480, 70, CENTER, CENTER), "Gui scale ", &guiScale, 0, 10))
					{
						SetGuiScale((float)(guiScale - 5) / 20 + 1);
					}

					if(GuiButtonSfx(GetRectanglePlacement(0, 160, 600, 70, CENTER, CENTER), "Toggle fullscreen"))
					{
						ToggleFullscreen();
					}

					if(GuiButtonSfx(GetRectanglePlacement(0, 240, 600, 70, CENTER, CENTER), "Close"))
					{
						SetTimeSpeed(1);
						showOptions = false;
					}
				}
				else if(GuiButtonSfx(GetRectanglePlacement(10, -10, 140, 80, LEFT, BOTTOM), "Options"))
				{
					SetTimeSpeed(0);
					showOptions = true;
				}
			}
		EndDrawing();

	}

	UnloadAssets();
	TerminateGameContext();
	return 0;
}

void HandleInitialScreen()
{
	if(!Timer_HasBeenSet(timer)) Timer_Set(&timer, 2);

	DrawTextPro(
		font,
		GAME_NAME,
		GetVector2Placement(0, 0, CENTER, CENTER),
		GetTextOffset(font, GAME_NAME, 160, 1, CENTER, CENTER),
		0, 160, 1,
		textColorFocused
	);

	if(Timer_HasEnded(timer))
	{
		phase = MAIN_MENU;
		Timer_Set(&timer, 0);
		PlaySound(sounds[DING_SOUND]);
	}
}

void HandleMainMenu()
{
	static int boardSize = MIN_FIELD_COUNT;
	static int playerCount = 1;
	static int botCount = 1;

	DrawTextPro(
		font,
		GAME_NAME,
		GetVector2Placement(0, 120, CENTER, TOP),
		GetTextOffset(font, GAME_NAME, 160, 1, CENTER, TOP),
		0, 160, 1,
		textColorFocused
	);
	if(GuiButtonSfx(GetRectanglePlacement(-310,0,300,100,CENTER,CENTER), "PLAY"))
	{
		SetupBoard(&board, boardSize, playerCount, botCount);
		phase = BOARD;
	}
	if(GuiButtonSfx(GetRectanglePlacement(0,0,300,100,CENTER,CENTER), "HOW TO PLAY"))
	{
		phase = HOW_TO_PLAY;
	}
	if(GuiButtonSfx(GetRectanglePlacement(310,0,300,100,CENTER,CENTER), "EXIT"))
	{
		phase = EXIT_SCREEN;
	}
	if(GuiSpinnerSfx(GetRectanglePlacement(0,95,500,70,CENTER,CENTER), "Board size ", &boardSize, MIN_FIELD_COUNT, MAX_FIELD_COUNT))
	{
		switch(boardSize % 4)
		{
			case 1:
				boardSize += 3;
				break;
			case 3:
				boardSize -= 3;
				break;
			default:
				break;
		}
	}
	if(GuiSpinnerSfx(GetRectanglePlacement(0,175,500,70,CENTER,CENTER), "Player count ", &playerCount, 1, 4))
	{
		if(playerCount == 1 && botCount == 0) botCount = 1;
		else if(playerCount + botCount > 4) botCount = 4 - playerCount;
	}
	if(GuiSpinnerSfx(GetRectanglePlacement(0,255,500,70,CENTER,CENTER), "Bot count ", &botCount, 0, 3))
	{
		if(botCount == 0 && playerCount < 2) playerCount = 2;
		else if(playerCount + botCount > 4) playerCount = 4 - botCount;
	}
}

void HandleBoard()
{
	UpdateBoard(&board);
	if(HasGameEndedBoard(board))
	{
		phase = MAIN_MENU;
	}
}

#define SLIDE_COUNT 5
static const int slideLengths[SLIDE_COUNT] = { 15, 8, 15, 12, 8 };
void HandleHowToPlay()
{
	static int slideId = 0;
	if(!Timer_HasBeenSet(timer))
	{
		SetCameraFov(2);
		Timer_Set(&timer, slideLengths[slideId]);
	}

	const char *howToPlayText = "HOW TO PLAY";
	DrawTextPro(
		font,
		howToPlayText ,
		GetVector2Placement(0, 30, CENTER, TOP),
		GetTextOffset(font, howToPlayText , 100, 1, CENTER, TOP),
		0, 100, 1,
		textColorFocused
	);
	
	switch(slideId)
	{
		case 0:
			GuiBoxText(GetRectanglePlacement(0, 70, 800, 560, CENTER, CENTER),
				"The goal of the game is to dominate the other players.\nThere are three ways to achieve this:\n\n1. Monopolise three regions by buying up\nall the properties in them\n\n2. Force the other players into bankruptcy\n\n3. Collect as much money as possible\nbefore the decision is made to end the game"
			);
			break;
		case 1:
			GuiBoxText(GetRectanglePlacement(0, -100, 800, 100, CENTER, CENTER), "Players can construct buildings on their own fields");
			
			DrawTextPro(
				font,
				"Construction site",
				GetVector2Placement(-430, 180, CENTER, CENTER),
				GetTextOffset(font, "Construction site", 35, 1, CENTER, TOP),
				0, 35, 1,
				textColorNormal
			);
			DrawTextPro(
				font,
				"House",
				GetVector2Placement(-143, 180, CENTER, CENTER),
				GetTextOffset(font, "House", 35, 1, CENTER, TOP),
				0, 35, 1,
				textColorNormal
			);
			DrawTextPro(
				font,
				"Villa",
				GetVector2Placement(143, 180, CENTER, CENTER),
				GetTextOffset(font, "Villa", 35, 1, CENTER, TOP),
				0, 35, 1,
				textColorNormal
			);
			DrawTextPro(
				font,
				"Hotel",
				GetVector2Placement(430, 180, CENTER, CENTER),
				GetTextOffset(font, "Hotel", 35, 1, CENTER, TOP),
				0, 35, 1,
				textColorNormal
			);
			BeginMode3D(GetCamera3D());
			DrawModel(models[SITE_MODEL], (Vector3){-1, 0, -0.5}, 0.1, RAYWHITE);
			DrawModel(models[HOUSE_MODEL], (Vector3){-0.5, 0, 0}, 0.1, RAYWHITE);
			DrawModel(models[VILLA_MODEL], (Vector3){0, 0, 0.5}, 0.1, RAYWHITE);
			DrawModel(models[APARTAMENT_MODEL], (Vector3){0.5f, 0, 1}, 0.1, RAYWHITE);
			EndMode3D();
			break;
		case 2:
			GuiBoxText(GetRectanglePlacement(0, 70, 800, 560, CENTER, CENTER),
				"There are several special fields:\n\nStart - gives money when walked through\n\nChance - causes a random event to happen\n\nTaxation - takes a few per cent of the money\n\nLottery - takes a few per cent of the money from\nevery player and gives them to the random one\n\nPoliceman - stops you for a few turns"
			);
			break;
		case 3:
			GuiBoxText(GetRectanglePlacement(0, 70, 800, 560, CENTER, CENTER),
				"A player who has entered another\nplayer's field must pay a fee.\n\nIf a player runs out of money to pay the fee,\ntheir properties will start to be sold off\nuntil they have enough money.\n\nThe properties will be sold off from\nthe cheapest to the most expensive"
			);
			break;
		case 4:
			GuiBoxText(GetRectanglePlacement(0, 70, 800, 560, CENTER, CENTER),
				"A player can buy an unoccupied field\nor another player's field after paying them a fee.\n\nIf they have enough money,\nthey can upgrade it straight away\n\nAn upgraded square has a higher value\nand requires a higher parking fee"
			);
			break;
	}

	if(Timer_HasEnded(timer))
	{
		slideId++;
		PlaySound(sounds[DING_SOUND]);
		Timer_Set(&timer, 0);
		if(slideId >= SLIDE_COUNT)
		{
			phase = MAIN_MENU;
			slideId = 0;
		}
	}
}

void HandleExitScreen()
{
	if(!Timer_HasBeenSet(timer)) Timer_Set(&timer, 3);

	DrawTextPro(
		font,
		GAME_NAME,
		GetVector2Placement(0, 120, CENTER, TOP),
		GetTextOffset(font, GAME_NAME, 160, 1, CENTER, TOP),
		0, 160, 1,
		textColorFocused
	);

	const char* thanks = "THANK YOU FOR PLAYING";
	DrawTextPro(
		font,
		thanks,
		GetVector2Placement(0, 0, CENTER, CENTER),
		GetTextOffset(font, thanks, 60, 1, CENTER, TOP),
		0, 60, 1,
		textColorNormal
	);
	
	const char* credits1 = "Programming, game\nand sound design:\n\nJakub Siwek";
	DrawTextPro(
		font,
		credits1,
		GetVector2Placement(-80, 140, CENTER, CENTER),
		GetTextOffset(font, credits1, 30, 1, RIGHT, TOP),
		0, 30, 1,
		textColorNormal
	);

	const char* credits2 = "\n3D model design:\n\nJakub Gora";
	DrawTextPro(
		font,
		credits2,
		GetVector2Placement(80, 140, CENTER, CENTER),
		GetTextOffset(font, credits2, 30, 1, LEFT, TOP),
		0, 30, 1,
		textColorNormal
	);

	if(Timer_HasEnded(timer))
	{
		shouldClose = true;
		Timer_Set(&timer, 0);
	}
}
