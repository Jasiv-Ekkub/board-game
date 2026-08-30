#include <board_rendering.h>
#include <board_setup.h>
#include <stdio.h>

void DebugAction(Board* board)
{
	AddPopupBoard(board, "Big big shot");
	AddPopupBoard(board, "Be a big, be a big");
	AddPopupBoard(board, "Now its your chance to be a big shot");
}

void DebugSuperaction(Board* board)
{
	Player* player = &board->players[board->currentPlayer];
	//Field* field = &board->fields[player->position];

	player->money += 1000;
	PlaySound(sounds[KA_CHING_SOUND]);
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

const Color playerColors[MAX_PLAYER_COUNT] = {
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
