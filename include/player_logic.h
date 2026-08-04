#ifndef PLAYER_LOGIC_H
#define PLAYER_LOGIC_H

#include <board.h>
#include <game_context.h>
#include <player.h>

typedef enum PlayerResponse
{
	 NONE,
	 POSITIVE,
	 NEGATIVE,
} PlayerResponse;

PlayerResponse GetPlayerResponse(Player player, BoardPhase phase, GameContext gameContext);

#endif //PLAYER_LOGIC_H
