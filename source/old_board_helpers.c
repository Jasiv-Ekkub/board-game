#include <old_board_helpers.h>
#include <stdio.h>
#include <string.h>

/*
Vector3 CalculatePlayerPosition(Board board, int playerId)
{
	Player player = board.players[playerId];
	float pawnOffset = board.graphics.pawnOffset;

	Vector2 pixelPosition = CalculateFieldCenter(board, player.position);
	
	pixelPosition.x += (playerId&1 ? pawnOffset : -pawnOffset);
	pixelPosition.y += (playerId&2 ? pawnOffset : -pawnOffset);

	return (Vector3){
		board.position.x + (pixelPosition.x / board.graphics.pixelSize * 2 - 1),
		board.position.y,
		board.position.z + (pixelPosition.y / board.graphics.pixelSize * 2 - 1),
	};
}

Vector3 CalculateHousePosition(Board board, int fieldId)
{
	int half = board.fieldCount/2;
	int quarter = half/2;

	BoardGraphics graphics = board.graphics;

	Vector2 pixelPosition = CalculateFieldCenter(board, fieldId);

	if(fieldId%quarter == 0)
	{
		pixelPosition.x += (graphics.cornerSize - graphics.dividerOffset)/2;
		pixelPosition.y -= (graphics.cornerSize - graphics.dividerOffset)/2;
	}
	else
	{
		if(fieldId%half > quarter)
		{
			pixelPosition.y -= (graphics.cornerSize - graphics.dividerOffset)/2;
		}
		else
		{
			pixelPosition.x += (graphics.cornerSize - graphics.dividerOffset)/2;
		}
	}

	return (Vector3){
		board.position.x + (pixelPosition.x / board.graphics.pixelSize * 2 - 1),
		board.position.y,
		board.position.z + (pixelPosition.y / board.graphics.pixelSize * 2 - 1),
	};
}
*/
