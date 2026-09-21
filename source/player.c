#include <player.h>
#include <string.h>
#include <helpers.h>

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

		.buyFieldRisk = GetRandomFloat(1, 3),
		.buyFieldRatio = GetRandomFloat(1, 3),

		.upgradeFieldRisk = GetRandomFloat(1, 5),
		.upgradeFieldRatio = GetRandomFloat(2, 8),
	};
	strncpy(player.name, name, PLAYER_NAME_LENGTH);
	return player;
}
