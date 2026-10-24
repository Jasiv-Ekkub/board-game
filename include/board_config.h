#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <field.h>

#define MAX_GROUP_DATA_LENGTH 16
#define MAX_GROUP_DATA_COUNT 16

typedef struct FieldData
{
    char name[FIELD_TEXT_LENGTH];
    int minCost;
    int maxCost;
} FieldData;

typedef struct GroupData
{
    Color color;
    int fieldDataCount;
    FieldData fieldDatas[MAX_GROUP_DATA_LENGTH];
} GroupData;

typedef struct BoardConfig
{
    int groupDataCount;
    GroupData groupDatas[MAX_GROUP_DATA_COUNT];
} BoardConfig;

BoardConfig GetBoardConfig();

#endif //BOARD_CONFIG_H
