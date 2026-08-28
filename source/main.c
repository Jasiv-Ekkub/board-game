#include <board.h>
#include <assets.h>
#include <game_context.h>
#include <raygui.h>
#include <layout_engine.h>
#include <gui_elements.h>
#include <stdbool.h>

enum { MAIN_MENU, BOARD } phase = MAIN_MENU;
bool shouldClose = false;
Board board;

void HandleMainMenu();
void HandleBoard();

int main()
{
	InitializeGameContext();
	LoadAssets();
	

	board = GetBoard();


	Color bgrColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
	while(!shouldClose)
	{
		shouldClose = WindowShouldClose();
		UpdateGameContext();
		if(IsKeyPressed(KEY_F4)) ToggleFullscreen();

		BeginDrawing();
			ClearBackground(bgrColor);
			switch(phase)
			{
				case MAIN_MENU:
					HandleMainMenu();
					break;
				case BOARD:
					HandleBoard();
					break;
			}
		EndDrawing();

	}

	UnloadAssets();
	TerminateGameContext();
	return 0;
}


void HandleMainMenu()
{
	static int boardSize = MIN_FIELD_COUNT;
	static int playerCount = 1;
	static int botCount = 1;

	if(GuiButtonSfx(GetRectanglePlacement(-155,0,300,100,CENTER,CENTER), "PLAY"))
	{
		SetupBoard(&board, boardSize, playerCount, botCount);
		phase = BOARD;
	}
	if(GuiButtonSfx(GetRectanglePlacement(155,0,300,100,CENTER,CENTER), "EXIT"))
	{
		shouldClose = true;
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
