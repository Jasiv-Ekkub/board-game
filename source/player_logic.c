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

	Rectangle messageBounds = GetRectanglePlacement(0,0, 600, 300, CENTER, CENTER);
	char buffer[TEXT_BUFFER_SIZE];
	char *options;
	
	switch(board->phase)
	{
		case ROLL_DICE:
			snprintf(buffer, TEXT_BUFFER_SIZE, "Roll dice");
			options = "Okay";
			break;
	
		case BUY_FIELD:
			snprintf(buffer, TEXT_BUFFER_SIZE, "Do you want to buy %s for $%i", field.name, field.value);
			options = "Yes;No";
			break;
	
		case UPGRADE_BUILDING:
			snprintf(buffer, TEXT_BUFFER_SIZE, "Do you want to upgrade %s\nin %s to %s for $%i", buildingLevelNames[field.buildingLevel], field.name, buildingLevelNames[field.buildingLevel+1], GetUpgradeValue(field));
			options = "Yes;No";
			break;

		default:
			snprintf(buffer, TEXT_BUFFER_SIZE, "Error");
			options = "Okay";
			break;
	}

	GuiPlayerInfo(GetRectanglePlacement(0, 150, 225, 100, CENTER, TOP), player);
	int response = GuiMessageBoxSfx(messageBounds, buffer, options);
	
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
