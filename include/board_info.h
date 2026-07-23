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

extern size_t fieldRegistrySize;
extern FieldInfo fieldRegistry[256];

void LoadBoardInfo(const char* filename);

void GetFieldComment(char* dst, int n, FieldInfo fieldInfo);

#endif //BOARD_INFO_H
