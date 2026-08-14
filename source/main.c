#include <board.h>
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_MESSAGEBOX_BUTTON_HEIGHT 60
#include <raygui.h>
#include <layout_engine.h>
#include <gui_elements.h>
#include <style_buisness.h>
#include <stdbool.h>
#define BUFFER_SIZE 32

#define MAX(p,q) (p>q ? p : q)
#define MIN(p,q) (p<q ? p : q)

enum {
	MAIN_MENU = 0,
	BOARD,
	SUMMARY
} scene;

void HandleMainMenu(Board* board, GameContext gameContext);
void HandleBoard(Board* board, GameContext gameContext);
void HandleSummary(Board* board, GameContext gameContext);

Timer timer;
bool shouldClose = false;

int main()
{
	InitWindow(1440, 810, "Board game");
	SetExitKey(0);
	SetTargetFPS(60);
	GuiLoadStyleBuisness();

	GameContext gameContext = LoadGameContext();
	Board board = LoadBoard();
	
	timer = GetTimer(TIMER_SINGLE_PULSE | TIMER_REPEATING);
	SetTimer(&timer, 5);

	Color bgrColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));

	while(!shouldClose)
	{
		UpdateGameContext(&gameContext);
		if(IsKeyPressed(KEY_F4)) ToggleFullscreen();
		else if(IsKeyPressed(KEY_F6)) gameContext.guiScale += 0.05f;
		else if(IsKeyPressed(KEY_F7)) gameContext.guiScale -= 0.05f;

		BeginDrawing();
			ClearBackground(bgrColor);
			switch(scene)
			{
				case MAIN_MENU:
					HandleMainMenu(&board, gameContext);
					break;

				case BOARD:
					HandleBoard(&board, gameContext);
					break;

				case SUMMARY:
					HandleSummary(&board, gameContext);
					break;
			}

		EndDrawing();
	}

	UnloadBoard(board);	
	UnloadGameContext(gameContext);
	CloseWindow();
	return 0;
}


int boardSize = MIN_FIELD_AMOUNT;
int humanCount = 0;
int botCount = 0;

void HandleMainMenu(Board* board, GameContext gameContext)
{
	if(GuiButton(GetRectanglePlacement(-205,0,400,100,CENTER,CENTER,gameContext), "Play"))
	{
		SetupBoard(board, boardSize, humanCount, botCount);
		scene = BOARD;
	}
	if(GuiButton(GetRectanglePlacement(205,0,400,100,CENTER,CENTER,gameContext), "Exit game"))
	{
		shouldClose = true;
	}
	GuiSpinner(GetRectanglePlacement(0,100,300,80,CENTER,CENTER,gameContext), "Board size ", &boardSize, MIN_FIELD_AMOUNT, MAX_FIELD_AMOUNT, false);
	if(boardSize%4 == 1) boardSize += 3;
	else if(boardSize%4 == 3) boardSize -= 3;
	GuiSpinner(GetRectanglePlacement(0,190,300,80,CENTER,CENTER,gameContext), "Human players ", &humanCount, 1, 4, false);
	GuiSpinner(GetRectanglePlacement(0,280,300,80,CENTER,CENTER,gameContext), "Bot players ", &botCount, MAX(0,2-humanCount), 4 - humanCount, false);
}

void HandleBoard(Board* board, GameContext gameContext)
{
	UpdateBoard(board, gameContext);

	if(HasGameEnded(*board))
	{
		char buffer[BUFFER_SIZE];
		int gameTime = timer.currentTime;
		snprintf(buffer, BUFFER_SIZE, "%02i:%02i", gameTime/60, gameTime%60);
		GuiBoxText(GetRectanglePlacement(0, 0, 225, 75, CENTER, CENTER, gameContext), buffer);
		if(UpdateTimer(&timer, gameContext))
		{
			scene = SUMMARY;
		}
	}
	else if(GuiButton(GetRectanglePlacement(10,0,100,60,LEFT,CENTER,gameContext), "Finish"))
	{
		scene = SUMMARY;
	}

}

void HandleSummary(Board* board, GameContext gameContext)
{
	char buffer[BUFFER_SIZE];
	int gameTime = timer.currentTime;
	snprintf(buffer, BUFFER_SIZE, "%02i:%02i", gameTime/60, gameTime%60);
	GuiBoxText(GetRectanglePlacement(0, 20, 225, 75, CENTER, TOP, gameContext), buffer);

	GuiBoxText(GetRectanglePlacement(0, -50, 225, 75, CENTER, CENTER, gameContext), "The winner is:");
	GuiPlayerInfo(GetRectanglePlacement(0, 50, 300, 100, CENTER, CENTER, gameContext), GetWinner(*board));
	
	if(UpdateTimer(&timer, gameContext))
	{
		scene = MAIN_MENU;
	}
}
