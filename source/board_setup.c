#include <board_rendering.h>
#include <board_setup.h>
#include <stdio.h>

void StartSuperaction(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

	player->money += 1500;
	AddPopupBoard(board, "Earned $1500 for walking\nthrough start");
	PlaySound(sounds[KA_CHING_SOUND]);
}

void TaxationAction(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

	player->money -= 100;
	AddPopupBoard(board, "Player paid $100 in tax");
	PlaySound(sounds[KA_CHING_SOUND]);
}

void LotteryAction(Board* board)
{
	AddPopupBoard(board, "Lottery");
}

void PoliceAction(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

	player->turnSkips += 2;
	AddPopupBoard(board, "Player got sentenced\nto two turn skips");
}
 
void ChanceAction(Board* board)
{
	AddPopupBoard(board, "Chance");
}

static const struct {
	Color color;
	const char* cityNames[8];
} groupData[8] = {
	{
		YELLOW,
		{ "Frankfurt", "Berlin", "Munchen", "Hamburg", "Stuttgart", "Hanover", "Augsburg", "Dortmund", },
	},
	{
		RED,
		{ "Szczecin", "Warszawa", "Krakow", "Częstochowa", "Pszczyna", "Torun", "Gdansk", "Katowice", }
	},
	{
		BLACK,
		{ "Copenhagen", "Stockholm", "Oslo", "Helsinki", "Bilund", "Bergen", "Gothenburg", "Turku", },
	},
	{
		GRAY,
		{ "Athens", "Ljubljana", "Zagreb", "Sarajevo", "Belgrad", "Kosovo", "Tirane", "Sofia", },
	},
	{
		BLUE,
		{ "Lyon", "Paris", "Rennes", "Nantes", "Orleans", "Toulouse", "Marseille", "Nice", },
	},
	{
		PURPLE,
		{ "Dublin", "London", "Glasgow", "Edinburgh", "Manchester", "Belfast", "Liverpool", "Manchester", },
	},
	{
		ORANGE,
		{ "Madrid", "Lisbon", "Valencia", "Porto", "Seville", "Zaragoza", "Barcelona", "Malaga", },
	},
	{
		GREEN,
		{ "Venice", "Rome", "Naples", "Florence", "Genoa", "Bologna", "Palermo", "Turin", },
	},
};

void LoadFields(Board* board)
{
	int innerCounter = 0;
	int outerCounter = -1;
	int quarter = board->fieldCount/4;
	for(int i=0; i<board->fieldCount; ++i)
	{
		int qi = i%quarter;
		if(qi == 0)
		{
			switch(i/quarter)
			{
				case 0:
					board->fields[i] = GetSuperactionField("Start", "Get salary", StartSuperaction, START_IMAGE);
					break;
				
				case 1:
					board->fields[i] = GetActionField("Taxation", "Pay tax", TaxationAction, START_IMAGE);
					break;
				
				case 2:
					board->fields[i] = GetActionField("Lottery", "Chance for win", LotteryAction, START_IMAGE);
					break;

				default:
					board->fields[i] = GetActionField("Police", "Get arrested", PoliceAction, START_IMAGE);
					break;
			}
			innerCounter = 0;
			outerCounter++;
		}
		else if(qi == quarter/2)
		{
			board->fields[i] = GetActionField("Chance", "Draw a card", ChanceAction, START_IMAGE);
			innerCounter = 0;
			outerCounter++;
		}
		else
		{

			board->fields[i] = GetPropertyField(
				groupData[outerCounter].cityNames[innerCounter],
				100 * (i / 3 + 3),
				outerCounter,
				groupData[outerCounter].color
			);
			if(innerCounter < 7) innerCounter++;
		}
	}
}

static const Color playerColors[MAX_PLAYER_COUNT] = {
	RED,
	YELLOW,
	GREEN,
	BLUE
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
