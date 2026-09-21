#include <player_logic.h>
#include <layout_engine.h>
#include <raygui.h>
#include <gui_elements.h>
#include <stdio.h>
#include <helpers.h>

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

	Rectangle messageBounds = GetRectanglePlacement(0,0, 600, 220, CENTER, CENTER);
	char buffer[TEXT_BUFFER_SIZE];
	char *options;
	
	switch(board->phase)
	{
		case ROLL_DICE:
			snprintf(buffer, TEXT_BUFFER_SIZE, "Roll dice");
			options = "Okay";
			break;
	
		case BUY_FIELD:
			snprintf(buffer, TEXT_BUFFER_SIZE, "Do you want to buy %s for $%i", field.name, GetFieldValue(field));
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
	if(Timer_HasEnded(*timer)) Timer_Set(timer, 1);
	Timer_Update(timer);
	if(!Timer_HasEnded(*timer))
	{
		board->currentPlayerResponse = NONE;
		return false;
	}
	
	Player* player = &board->players[board->currentPlayer];
	Field field = board->fields[player->position];

	float likelihood = 0;
	float costRatio = 0;
	switch(board->phase)
	{
		case ROLL_DICE:
			return true;
		
		case BUY_FIELD:
			costRatio = (float)(player->money)/GetFieldValue(field);
			likelihood = Sigmoid(player->buyFieldRisk * (costRatio - player->buyFieldRatio));
			break;

		case UPGRADE_BUILDING:
			costRatio = (float)(player->money)/GetUpgradeValue(field);
			likelihood = Sigmoid(player->upgradeFieldRisk * (costRatio - player->upgradeFieldRatio));
			break;

		default:
			break;
	}

	if(likelihood >= GetRandomFloat(0, 1)) board->currentPlayerResponse = POSITIVE;
	else board->currentPlayerResponse = NEGATIVE;

	return true;
}
