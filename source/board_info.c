#include <board_info.h>
#include <stdio.h>
#include <string.h>

BoardInfo boardInfo = {0};

void AddFieldEntry(FieldInfo entry)
{
	if(boardInfo.fieldRegistrySize < 255)
	{
		boardInfo.fieldRegistry[boardInfo.fieldRegistrySize++] = entry;
	}
}

void LoadBoardInfo(const char* filename)
{
	for(size_t i=0; i<20; ++i)
	{
		FieldInfo fieldInfo = {0};
		snprintf(fieldInfo.name, 20, "City %i", i);
		fieldInfo.cost = i * 50 + 200;
		fieldInfo.color = RED;

		AddFieldEntry(fieldInfo);
	}

	boardInfo.graphicalInfo = (BoardGraphicalInfo){
		.border = 5,
		.divider = 40,
		.width = 100,
		.height = 180,

		.fontSize = 20,
		.fontSpacing = 1,

		.nameOffset = 40,
		.commentOffset = 18,

		.boardLight = WHITE,
		.boardDark = BLACK
	};
}

void GetFieldComment(char* dst, int n, FieldInfo fieldInfo)
{
	snprintf(dst, n, "$%i", fieldInfo.cost);
}
