#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <timer.h>
#include <stdbool.h>

#define PLAYER_NAME_LENGTH 32

typedef enum PlayerType
{
	HUMAN,
	BOT,
} PlayerType;

typedef enum PlayerResponse
{
	 NONE,
	 POSITIVE,
	 NEGATIVE,
} PlayerResponse;

typedef struct Player
{
	char name[PLAYER_NAME_LENGTH];
	PlayerType type;
	int money;
	int turnSkips;
	bool isBankrupt;

	int position;
	Color color;
	Vector3 modelPosition;

	//Bot data
	Timer timer;

	float buyFieldRisk;
	float buyFieldRatio;
	
	float upgradeFieldRisk;
	float upgradeFieldRatio;
} Player;

Player GetHumanPlayer(const char* name, Color color, int money);
Player GetBotPlayer(const char* name, Color color, int money);

#endif //PLAYER_H
