#include <player.h>
#include <string.h>

Player GetPlayer(const char* name, Color color)
{
	Player player = {
		.position = 0,
		.color = color,
	};
	strncpy(player.name, name, PLAYER_NAME_LENGTH);
	return player;
}
