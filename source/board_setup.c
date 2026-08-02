#include <board_setup.h>

void LoadFields(Board* board)
{
	int quarter = board->fieldCount/3;
	for(int i=0; i<board->fieldCount; ++i)
	{
		int qi = i%quarter;
		if(qi)
		{
			board->fields[i] = GetPropertyField("Wasteland", 0, GRAY);
			board->fields[i].ownerId = i%5 - 1;
			if(!(i&5)) continue;
			board->fields[i].buildingLevel = i%4;
		}
		else
		{
			board->fields[i] = GetActionField("Chance", "Draw a card", START_IMAGE);
		}
	}
}

void LoadPlayers(Board* board, int startMoney)
{

}
