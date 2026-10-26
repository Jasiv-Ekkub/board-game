#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <field.h>

#define MAX_CITY_COUNT 16
#define MIN_CITY_COUNT 4

#define MAX_GROUP_COUNT 16
#define MIN_GROUP_COUNT 8

typedef struct City
{
    char name[FIELD_TEXT_LENGTH];
    int minCost;
    int maxCost;
} City;

typedef struct Group
{
    Color color;
    int cityCount;
    City cities[MAX_CITY_COUNT];
} Group;

typedef struct BoardConfig
{
    int groupCount;
    Group groups[MAX_GROUP_COUNT];
} BoardConfig;

void PrintConfig(BoardConfig config);

BoardConfig GetBoardConfig();

#endif //BOARD_CONFIG_H
