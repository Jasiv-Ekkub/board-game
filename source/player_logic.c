#include <player_logic.h>
#include <layout_engine.h>
#include <raygui.h>

bool GetHumanPlayerResponse(Board* board, GameContext gameContext);
bool GetBotPlayerResponse(Board* board, GameContext gameContext);

bool GetPlayerResponse(Board* board, GameContext gameContext)
{
	Player player = board->players[board->currentPlayer];
	switch(player.type)
	{
		case HUMAN:
			return GetHumanPlayerResponse(board, gameContext);

		case BOT:
			return GetBotPlayerResponse(board, gameContext);
		
		default:
		 	return false;
	}
}

bool GetHumanPlayerResponse(Board* board, GameContext gameContext)
{
	switch(board->phase)
	{
		case ROLL_DICE:
			return GuiButton(GetRectanglePlacement(0, 0, 500, 100, CENTER, CENTER, gameContext), "Roll dice");

		default:
			return false;
	}
}

bool GetBotPlayerResponse(Board* board, GameContext gameContext)
{
	Timer* timer = &board->players[board->currentPlayer].timer;
	SetTimer(timer, 2);
	switch(board->phase)
	{
		case ROLL_DICE:
			return UpdateTimer(timer, gameContext);

		default:
			return false;
	}
	return false;
}
