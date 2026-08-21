#include <board_setup.h>
#include <stdio.h>

void DebugAction(Board* board)
{
	AddPopupBoard(board, "Now its your chance to be a big shot");
}

void DebugSuperaction(Board* board)
{
	Player* player = &board->players[board->currentPlayer];
	//Field* field = &board->fields[player->position];

	player->money += 1000;
}

void DebugSuperaction2(Board* board)
{
	//Player* player = &board->players[board->currentPlayer];
	//Field* field = &board->fields[player->position];

	//player->money -= 10000;
}

void LoadFields(Board* board)
{
	int quarter = board->fieldCount/4;
	for(int i=0; i<board->fieldCount; ++i)
	{
		int qi = i%quarter;
		if(i == 0)
		{
			board->fields[i] = GetSuperactionField("Start", "Get salary", DebugSuperaction, START_IMAGE);
		}
		else if(qi == 0)
		{
			board->fields[i] = GetActionField("Chance", "Draw a card", DebugAction, START_IMAGE);
		}
		else if(qi == 3)
		{
			board->fields[i] = GetSuperactionField("Foobar", "Sasalele", DebugSuperaction2, START_IMAGE);
		}
		else
		{
			board->fields[i] = GetPropertyField("Wasteland", 100, GetColor(0x303030FF));
			//board->fields[i].ownerId = 0;
			//board->fields[i].buildingLevel = i % MAX_BUILDING_LEVEL;
		}
	}
}

const Color playerColors[MAX_PLAYER_AMOUNT] = {
	RED,
	YELLOW,
	GREEN,
	BLUE
};

void LoadPlayers(Board* board, int startMoney, int humanCount, int botCount)
{
	if(humanCount > 4)
	{
		humanCount = 4;
		botCount = 0;
	}
	else if(botCount > 4 - humanCount)
	{
		botCount = 4 - humanCount;
	}

	board->playerCount = humanCount + botCount;

	char buffer[PLAYER_NAME_LENGTH];
	for(int i=0; i<MAX_PLAYER_AMOUNT; ++i)
	{
		if(i < humanCount)
		{
			snprintf(buffer, PLAYER_NAME_LENGTH, "Player %c", i+'A');
			board->players[i] = GetHumanPlayer(buffer, playerColors[i], startMoney);
		}
		else if(i - humanCount < botCount)
		{
			snprintf(buffer, PLAYER_NAME_LENGTH, "Bot %c", i+'A'-humanCount);
			board->players[i] = GetBotPlayer(buffer, playerColors[i], startMoney);
		}
		else
		{
			board->players[i] = GetBotPlayer("No player", GRAY, 0);
		}
	}
	board->currentPlayer = 0;
}
