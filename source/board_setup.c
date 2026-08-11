#include <board_setup.h>

void DebugAction(Player* player)
{
	player->money -= 100;
}

void DebugSuperaction(Player* player)
{
	player->money += 1000;
}

void DebugSuperaction2(Player* player)
{
	
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
			board->fields[i].ownerId = 0;
			board->fields[i].buildingLevel = i % MAX_BUILDING_LEVEL;
		}
	}
}

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

	for(int i=0; i<MAX_PLAYER_AMOUNT; ++i)
	{
		if(i < humanCount)
		{
			board->players[i] = GetHumanPlayer("Human", RED, startMoney);
		}
		else if(i - humanCount < botCount)
		{
			board->players[i] = GetBotPlayer("Bot", BLUE, startMoney);
		}
		else
		{
			board->players[i] = GetBotPlayer("No player", GRAY, 0);
		}
	}
	board->currentPlayer = 0;
}
