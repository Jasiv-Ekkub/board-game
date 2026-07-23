#ifndef BOARD_INFO_H
#define BOARD_INFO_H

#include <raylib.h>
#include <stddef.h>

typedef struct FieldInfo
{
	char name[20];
	int cost;
	Color color;
} FieldInfo;

void GetFieldComment(char* dst, int n, FieldInfo fieldInfo);

typedef struct BoardGraphicalInfo
{
	int border;
	int divider;
	int width;
	int height;

	float fontSize;
	float fontSpacing;

	int nameOffset;
	int commentOffset;

	Color boardLight;
	Color boardDark;
} BoardGraphicalInfo;

typedef struct BoardInfo
{
	BoardGraphicalInfo graphicalInfo;
	size_t fieldRegistrySize;
	FieldInfo fieldRegistry[256];
} BoardInfo;

extern BoardInfo boardInfo;
void LoadBoardInfo(const char* filename);

#endif //BOARD_INFO_H
