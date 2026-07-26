#include <board.h>
#include <board_helpers.h>
#include <string.h>
#include <stdio.h>


void DebugPrintAllFields(Board board)
{
	for(uint16_t i=0; i<board.fieldCount; ++i)
	{
		Field* field = board.fields + i;
		printf("name:%s, value:%i, buildingLevel:%i, ownerId:%i, color:%i\n", field->name, field->value, field->buildingLevel, field->ownerId, field->color);
	}
}

#warning LoadBoard: filename does nothing
Board LoadBoard(const char* filename, uint16_t size)
{
	if(size > 256) size = 256;
	else if(size < 16) size = 16;
	else size &= 0xFFFC;

	Board board = {
		.fieldCount = size,
	};
	
	LoadFieldsDefault(&board);
	LoadGraphicsDefault(&board);

	/*
	if(!strcmp(filename, "default"))	
	{
		
	}
	else
	{

	}
	*/

	board.graphics.boardTexture = LoadBoardTexture(board);
	return board;
}

void UpdateBoard(Board* board, GameContext gameContext)
{
	DrawTexturePro(board->graphics.boardTexture, (Rectangle){0, 0, board->graphics.boardTexture.width, board->graphics.boardTexture.height}, (Rectangle){5, 5, 800, 800}, (Vector2){0}, 0, WHITE);
}

void UnloadBoard(Board board)
{
	UnloadTexture(board.graphics.boardTexture);
	UnloadFont(board.graphics.font);
}
