#include <board_rendering.h>
#include <board_setup.h>
#include <board_config.h>
#include <stdio.h>
#include <stdlib.h>
#include <chance_action.h>

void StartSuperaction(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

	player->money += 500;
	AddPopupBoard(board, "Earned $500 for walking\nthrough start");
	PlaySound(sounds[KA_CHING_SOUND]);
}

void TaxationAction(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

	int money = player->money;
	player->money = money * 4 / 5;

	char buffer[MAX_POPUP_LENGTH];
	snprintf(buffer, MAX_POPUP_LENGTH, "Player paid $%i in tax", money - player->money);
	AddPopupBoard(board, buffer);
	PlaySound(sounds[KA_CHING_SOUND]);
}

void LotteryAction(Board* board)
{
	int funds = 0;
	for(int i=0; i<board->playerCount; ++i)
	{
		if(board->players[i].money < 0) continue;
		int tmp = board->players[i].money / 10;
		funds += tmp;
		board->players[i].money -= tmp;
	}
	int winner = 0;
	do
	{
		winner = rand() % board->playerCount;
	}
	while(board->players[winner].money < 0);
	board->players[winner].money += funds;

	char buffer[MAX_POPUP_LENGTH];
	snprintf(buffer, MAX_POPUP_LENGTH, "%s wins $%i", board->players[winner].name, funds);
	AddPopupBoard(board, buffer);

	AddPopupBoard(board, "Lottery - everybody gives 10\% as a bet");
	PlaySound(sounds[KA_CHING_SOUND]);
}

void PoliceAction(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

	player->turnSkips += 2;
	AddPopupBoard(board, "Player got sentenced\nto two turn skips");
}
 
void LoadFields(Board* board)
{
    const BoardConfig config = GetBoardConfig();
    PrintConfig(config);

    int groupIds[8] = {};
    int cityIds[8][4] = {};
    
    for(int i=0; i<8;)
    {
        int id = rand()%config.groupCount;
        bool reroll = false;
        for(int j=0; j<i; ++j)
        {
            if(groupIds[j] == id)
            {
                reroll = true;
                break;
            }
        }
        if(reroll) continue;

        groupIds[i] = id;
        const Group group = config.groups[id];

        for(int j=0; j<4;)
        {
            int id2 = rand()%group.cityCount;
            bool reroll2 = false;
            for(int k=0; k<j; ++k)
            {
                if(cityIds[i][k] == id2)
                {
                    reroll2 = true;
                    break;
                }
            }
            if(reroll2) continue;

            cityIds[i][j] = id2;
            j++;
        }
        i++;
    }

    int innerCounter = 0;
    int outerCounter = -1;

	int quarter = board->fieldCount/4;
	for(int i=0; i<board->fieldCount; ++i)
	{
        if(innerCounter > 4) innerCounter = 4;
        if(outerCounter > 8) outerCounter = 8;

		int qi = i%quarter;
		if(qi == 0)
		{
			switch(i/quarter)
			{
				case 0:
					board->fields[i] = GetSuperactionField("Start", "Get salary", StartSuperaction, START_IMAGE);
					break;
				
				case 1:
					board->fields[i] = GetActionField("Taxation", "Pay tax", TaxationAction, TAXATION_IMAGE);
					break;
				
				case 2:
					board->fields[i] = GetActionField("Lottery", "Chance for win", LotteryAction, LOTTERY_IMAGE);
					break;

				default:
					board->fields[i] = GetActionField("Police", "Get arrested", PoliceAction, POLICEMAN_IMAGE);
					break;
			}
            outerCounter++;
            innerCounter = 0;
		}
		else if(qi == quarter/2)
		{
			board->fields[i] = GetActionField("Chance", "Draw a card", ChanceAction, CHANCE_IMAGE);

            outerCounter++;
            innerCounter = 0;
		}
		else
		{
            const Group group = config.groups[groupIds[outerCounter]];
            const City city = group.cities[cityIds[outerCounter][innerCounter]];

            int q = city.minCost, p = city.maxCost;
            if(p < q)
            {
                int tmp = p;
                p = q;
                q = tmp;
            }
            p -= q - 1;

			board->fields[i] = GetPropertyField(
				city.name,
				rand() % p + q, 
				outerCounter,
				group.color
			);
            innerCounter++;
		}
	}
}

static const Color playerColors[MAX_PLAYER_COUNT] = {
	(Color){ 0xaf, 0x00, 0x00, 0xff },
	(Color){ 0xaf, 0xaf, 0x00, 0xff },
	(Color){ 0x00, 0xaf, 0x00, 0xff },
	(Color){ 0x00, 0x00, 0xaf, 0xff },
};

void LoadPlayers(Board* board, int startMoney, int humanCount, int botCount)
{
	board->playerCount = humanCount + botCount;

	char buffer[PLAYER_NAME_LENGTH];
	for(int i=0; i<MAX_PLAYER_COUNT; ++i)
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
	board->winnerId = -1;
}

void GenerateBoardTexture(Board* board)
{
	BoardLayout layout = board->layout;
	int quarter = board->fieldCount/4;

	float cornerOffset = layout.fieldHeight + layout.borderWidth * 2;
	float edgeOffset = layout.fieldWidth + layout.borderWidth;

	float sideOffset = cornerOffset + edgeOffset * (float)(quarter-1) - layout.borderWidth;
	float boardSize = sideOffset + cornerOffset;
	board->layout.boardSize = boardSize;
	board->layout.modelScale = layout.modelScaleMultiplier * layout.dividerOffset / boardSize;
	
	Image image = GenImageColor((int)boardSize, (int)boardSize, board->colors.dark);
	ImageDrawRectangleRec(&image, (Rectangle){cornerOffset, cornerOffset, sideOffset-cornerOffset, sideOffset-cornerOffset}, board->colors.light);

	//Down
	DrawBoardCorner(&image, *board, (Rectangle){
		layout.borderWidth,
		layout.borderWidth + sideOffset,
		layout.fieldHeight,
		layout.fieldHeight,
		}, 0);
	//Left
	DrawBoardCorner(&image, *board, (Rectangle){
		layout.borderWidth,
		layout.borderWidth,
		layout.fieldHeight,
		layout.fieldHeight,
		}, quarter);
	//Up
	DrawBoardCorner(&image, *board, (Rectangle){
		layout.borderWidth + sideOffset,
		layout.borderWidth,
		layout.fieldHeight,
		layout.fieldHeight,
		}, quarter * 2);
	//Right
	DrawBoardCorner(&image, *board, (Rectangle){
		layout.borderWidth + sideOffset,
		layout.borderWidth + sideOffset,
		layout.fieldHeight,
		layout.fieldHeight,
		}, quarter * 3);

	for(int i=1; i<quarter; ++i)
	{
		DrawBoardEdge(&image, *board, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth,
			layout.fieldWidth,
			layout.fieldHeight,
			}, quarter + i);
		
		DrawBoardEdge(&image, *board, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth + sideOffset,
			layout.fieldWidth,
			layout.fieldHeight,
			}, 4*quarter - i);
	}
	ImageRotateCCW(&image);
	for(int i=1; i<quarter; ++i)
	{
		DrawBoardEdge(&image, *board, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth,
			layout.fieldWidth,
			layout.fieldHeight,
			}, 2*quarter + i);
		
		DrawBoardEdge(&image, *board, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth + sideOffset,
			layout.fieldWidth,
			layout.fieldHeight,
			}, quarter - i);
	}
	ImageRotateCW(&image);

	UnloadTexture(textures[BOARD_TEXTURE]);
	textures[BOARD_TEXTURE] = LoadTextureFromImage(image);
	models[BOARD_MODEL].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textures[BOARD_TEXTURE];
	UnloadImage(image);
}
