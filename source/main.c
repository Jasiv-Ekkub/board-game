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
	[HOW_TO_PLAY] = true,
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

					static int guiScale = 0;
					if(GuiSpinnerSfx(GetRectanglePlacement(60, 80, 480, 70, CENTER, CENTER), "Gui scale ", &guiScale, 0, 10))
					{
						SetGuiScale((float)guiScale / 20 + 0.75);
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
		textColorNormal
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
		textColorNormal
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

#define SLIDE_COUNT 4
void HandleHowToPlay()
{
	static int slideId = 0;
	
	const char *howToPlayText = "HOW TO PLAY";
	DrawTextPro(
		font,
		howToPlayText ,
		GetVector2Placement(0, 30, CENTER, TOP),
		GetTextOffset(font, howToPlayText , 100, 1, CENTER, TOP),
		0, 100, 1,
		textColorNormal
	);
	
	const char *slideText = "";
	switch(slideId-1)
	{
		case 0:
			slideText = "The goal of the game is to achieve domination\n"
                        "over other players. Player can do this by:\n"
                        "1. Monopolizing three regions\n"
                        "2. Bankrupting other players\n"
                        "3. Accumulating largest wealth by the end of the game";
			break;
		case 1:
			slideText = "There are two types of fields:\n"
                        "1. Property fields, which can\nbe bought and developed\n"
                        "2. Special fields, which causes\nspecial event to occur:";
			break;
		case 2:
			slideText = "2.1. Start - gives $500\n"
                        "2.2. Taxation - takes 20\% of owned money\n"
                        "2.3. Lottery - takes 10\% of owned money from\n"
                             "every player and gives to one of them\n"
                        "2.4. Policeman - stops player for two rounds\n"
                        "2.5. Chance - causes random event";
			break;
		case 3:
			slideText = "Player must pay the fee,\n"
                        "when trespassing on someone's property.\n\n"
                        "The more developed it is\n"
                        "the larger the fee is.";
			break;
	}
	GuiBoxText(GetRectanglePlacement(0, 0, 800, 500, CENTER, CENTER), slideText);

	GuiSpinnerSfx(GetRectanglePlacement(0,-80,500,60,CENTER,BOTTOM), 0, &slideId, 1, SLIDE_COUNT);
	if(GuiButton(GetRectanglePlacement(0,-10,500,60,CENTER,BOTTOM), "RETURN"))
	{
		PlaySound(sounds[DING_SOUND]);
		phase = MAIN_MENU;
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
		textColorNormal
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

	const char* credits2 = "\n3D model design:\n\nJakub Gora\nJakub Siwek";
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
