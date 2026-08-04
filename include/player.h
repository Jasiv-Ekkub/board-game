#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#define PLAYER_NAME_LENGTH 32

typedef enum PlayerType
{
	HUMAN,
	BOT,
} PlayerType;

typedef struct Player
{
	char name[PLAYER_NAME_LENGTH];
	PlayerType type;

	int position;
	Color color;
} Player;

Player GetHumanPlayer(const char* name, Color color);
Player GetBotPlayer(const char* name, Color color);

#endif //PLAYER_H
