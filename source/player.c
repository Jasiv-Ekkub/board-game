#include <player.h>
#include <string.h>

Player GetHumanPlayer(const char* name, Color color)
{
	Player player = {
		.type = HUMAN,
		.position = 0,
		.color = color,
	};
	strncpy(player.name, name, PLAYER_NAME_LENGTH);
	return player;
}

Player GetBotPlayer(const char* name, Color color)
{
	Player player = {
		.type = BOT,
		.position = 0,
		.color = color,
		.timer = GetTimer(0),
	};
	strncpy(player.name, name, PLAYER_NAME_LENGTH);
	return player;
}
