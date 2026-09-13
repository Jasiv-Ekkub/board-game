#include <player.h>
#include <string.h>

Player GetHumanPlayer(const char* name, Color color, int money)
{
	Player player = {
		.type = HUMAN,
		.position = 0,
		.color = color,
		.money = money,
		.turnSkips = 0,
	};
	strncpy(player.name, name, PLAYER_NAME_LENGTH);
	return player;
}

Player GetBotPlayer(const char* name, Color color, int money)
{
	Player player = {
		.type = BOT,
		.position = 0,
		.color = color,
		.money = money,
		.turnSkips = 0,

		.timer = Timer_Get(),
	};
	strncpy(player.name, name, PLAYER_NAME_LENGTH);
	return player;
}
