#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#define PLAYER_NAME_LENGTH 32

typedef struct Player
{
	char name[PLAYER_NAME_LENGTH];
	int position;
	Color color;
} Player;

Player GetPlayer(const char* name, Color color);

#endif //PLAYER_H
