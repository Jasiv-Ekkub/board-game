#include <board_info.h>
#include <stdio.h>
#include <string.h>

size_t fieldRegistrySize = 0;
FieldInfo fieldRegistry[256] = {};

void LoadBoardInfo(const char* filename)
{
	for(size_t i=0; i<20; ++i)
	{
		FieldInfo fieldInfo = {0};
		snprintf(fieldInfo.name, 20, "City %i", i);
		fieldInfo.cost = i * 50 + 200;
		fieldInfo.color = RED;
		fieldRegistry[fieldRegistrySize++] = fieldInfo;
	}
}

void GetFieldComment(char* dst, int n, FieldInfo fieldInfo)
{
	snprintf(dst, n, "$%i", fieldInfo.cost);
}
