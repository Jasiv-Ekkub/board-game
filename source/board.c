#include <board.h>
#include <board_helpers.h>
#include <string.h>
#include <stdio.h>


void DebugPrintAllFields(Board board)
{
	for(uint16_t i=0; i<board.size; ++i)
	{
		Field* field = board.fields + i;
		printf("name:%s, value:%i, buildingLevel:%i, ownerId:%i, color:%i\n", field->name, field->value, field->buildingLevel, field->ownerId, field->color);
	}
}

#warning LoadBoard: filename does nothing
Board LoadBoard(const char* filename, uint16_t size)
{
	if(size > 256) size = 256;
	else size &= 0xFFFC;

	Board board = {
		.size = size,
	};
	
	LoadFieldsDefault(size, board.fields);

	/*
	if(!strcmp(filename, "default"))	
	{
		
	}
	else
	{

	}
	*/

	Image image = GenImageCellular(500, 500, 32);
	board.graphics.boardTexture = LoadTextureFromImage(image);
	UnloadImage(image);

	return board;
}

void UpdateBoard(Board* board, GameContext gameContext)
{
	DrawTexture(board->graphics.boardTexture, 10, 10, WHITE);
}

void UnloadBoard(Board board)
{
	UnloadTexture(board.graphics.boardTexture);
}
