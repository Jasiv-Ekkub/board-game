#include <player_logic.h>
#include <layout_engine.h>
#include <raygui.h>
#include <gui_elements.h>
#include <stdio.h>

bool GetHumanPlayerResponse(Board* board);
bool GetBotPlayerResponse(Board* board);

bool GetPlayerResponse(Board* board)
{
	Player player = board->players[board->currentPlayer];
	switch(player.type)
	{
		case HUMAN:
			return GetHumanPlayerResponse(board);

		case BOT:
			return GetBotPlayerResponse(board);
		
		default:
		 	return false;
	}
}

#define TEXT_BUFFER_SIZE 128

bool GetHumanPlayerResponse(Board* board)
{
	Player player = board->players[board->currentPlayer];
	Field field = board->fields[player.position];
	char buffer[TEXT_BUFFER_SIZE];
	int response;
	GuiPlayerInfo(GetRectanglePlacement(0, 150, 225, 100, CENTER, TOP), player);
	switch(board->phase)
	{
		case ROLL_DICE:
			response = GuiButton(GetRectanglePlacement(0, 0, 500, 100, CENTER, CENTER), "Roll dice") - 1;
			break;
	
		case BUY_FIELD:
			snprintf(buffer, TEXT_BUFFER_SIZE, "Do you want to buy %s for $%i", field.name, field.value);
			response = GuiMessageBox(GetRectanglePlacement(0,0, 800, 200, CENTER, CENTER), 0, buffer, "Yes;No");
			break;
	
		case UPGRADE_BUILDING:
			snprintf(buffer, TEXT_BUFFER_SIZE, "Do you want to upgrade %s\nin %s to %s for $%i", buildingLevelNames[field.buildingLevel], field.name, buildingLevelNames[field.buildingLevel+1], GetUpgradeValue(field));
			response = GuiMessageBox(GetRectanglePlacement(0,0, 800, 250, CENTER, CENTER), 0, buffer, "Yes;No");
			break;

		default:
			response = 0;
			break;
	}
	switch(response)
	{
		case 1:
			board->currentPlayerResponse = POSITIVE;
			return true;
		case 0:
		case 2:
			board->currentPlayerResponse = NEGATIVE;
			return true;

		default:
			board->currentPlayerResponse = NONE;
			return false;
	}
}

bool GetBotPlayerResponse(Board* board)
{
	Timer* timer = &board->players[board->currentPlayer].timer;
	SetTimer(timer, 1);
	switch(board->phase)
	{
		case ROLL_DICE:
			return UpdateTimer(timer);
		case BUY_FIELD:
		case UPGRADE_BUILDING:
			board->currentPlayerResponse = POSITIVE;
			return UpdateTimer(timer);
		default:
			break;
	}
	board->currentPlayerResponse = NONE;
	return false;
}
