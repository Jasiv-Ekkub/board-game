#include <player_logic.h>
#include <layout_engine.h>
#include <raygui.h>
#include <gui_elements.h>
#include <stdio.h>

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
	Player player = board->players[board->currentPlayer];
	Field field = board->fields[player.position];
	switch(board->phase)
	{
		case ROLL_DICE:
			return GuiButton(GetRectanglePlacement(0, 0, 500, 100, CENTER, CENTER, gameContext), "Roll dice");
	
		case BUY_FIELD:
			char buffer[64];
			snprintf(buffer, 64, "Do you want to buy %s for $%i", field.name, field.value);
			int response = GuiMessageBox(GetRectanglePlacement(0,0, 800, 200, CENTER, CENTER, gameContext), 0, buffer, "Yes;No");
			switch(response)
			{
				case 0:
					return true;
				case 1:
					board->currentPlayerResponse = POSITIVE;
					return true;
				case 2:
					board->currentPlayerResponse = NEGATIVE;
					return true;
				default:
					break;
			}
			return false;
		default:
			return false;
	}
}

bool GetBotPlayerResponse(Board* board, GameContext gameContext)
{
	Timer* timer = &board->players[board->currentPlayer].timer;
	SetTimer(timer, 1);
	switch(board->phase)
	{
		case ROLL_DICE:
			return UpdateTimer(timer, gameContext);
		case BUY_FIELD:
			board->currentPlayerResponse = POSITIVE;
			return UpdateTimer(timer, gameContext);
		default:
			return false;
	}
	return false;
}
