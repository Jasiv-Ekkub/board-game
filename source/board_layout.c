#include <board_layout.h>
#include <board.h>

BoardLayout GetBoardLayout()
{
	BoardLayout layout = {
	.fieldWidth = 300,
	.fieldHeight = 450,
	
	.borderWidth = 8,
	.dividerOffset = 100,
	.pawnOffset = 40,

	.modelScaleMultiplier = 0.9f,

	.fontSize = 49,
	.fontSpacing = 0,

	.nameOffsetEdge = 140,
	.commentOffsetEdge = 200,
	.imageSizeEdge = 200,

	.nameOffsetCorner = 105,
	.commentOffsetCorner = 150,
	.imageSizeCorner = 320,
	};

	return layout;
}

Vector2 CalculateFieldCenter(Board board, int fieldNumber)
{
	BoardLayout layout = board.layout;
	int quarter = board.fieldCount/4;
	float cornerOffset = layout.borderWidth + layout.fieldHeight/2;
	
	Vector2 position = {0};

	int sideId = fieldNumber/quarter;
	int sideFieldId = fieldNumber%quarter;
	if(sideFieldId == 0)
	{
		switch(sideId)
		{
			case 0:
				position = (Vector2){
					cornerOffset,
					layout.boardSize - cornerOffset,
				};
				break;
			case 1:
				position = (Vector2){
					cornerOffset,
					cornerOffset,
				};
				break;
			case 2:
				position = (Vector2){
					layout.boardSize - cornerOffset,
					cornerOffset,
				};
				break;
			case 3:
				position = (Vector2){
					layout.boardSize - cornerOffset,
					layout.boardSize - cornerOffset,
				};
				break;
		}
	}
	else
	{
		float edgeOffsetBase = cornerOffset*2 + layout.fieldWidth/2;
		float edgeOffset = layout.fieldWidth + layout.borderWidth;

		float edgeOffsetFinal =  edgeOffsetBase + edgeOffset*(float)(sideFieldId - 1);
		switch(sideId)
		{
			case 0:
				position = (Vector2){
					cornerOffset,
					layout.boardSize - edgeOffsetFinal,
				};
				break;
			case 1:
				position = (Vector2){
					edgeOffsetFinal,
					cornerOffset,
				};
				break;
			case 2:
				position = (Vector2){
					layout.boardSize - cornerOffset,
					edgeOffsetFinal,
				};
				break;
			case 3:
				position = (Vector2){
					layout.boardSize - edgeOffsetFinal,
					layout.boardSize - cornerOffset,
				};
				break;
		}
	}

	return position;
}

Vector3 CalculatePlayerPosition(Board board, int playerId)
{
	Player player = board.players[playerId];
	float pawnOffset = board.layout.pawnOffset;

	Vector2 pixelPosition = CalculateFieldCenter(board, player.position);
	
	pixelPosition.x += (playerId&1 ? pawnOffset : -pawnOffset);
	pixelPosition.y += (playerId&2 ? pawnOffset : -pawnOffset);

	return (Vector3){
		board.position.x + (pixelPosition.x / board.layout.boardSize * 2 - 1),
		board.position.y,
		board.position.z + (pixelPosition.y / board.layout.boardSize * 2 - 1),
	};
}

Vector3 CalculateHousePosition(Board board, int fieldId)
{
	int half = board.fieldCount/2;
	int quarter = half/2;

	BoardLayout layout = board.layout;

	Vector2 pixelPosition = CalculateFieldCenter(board, fieldId);

	if(fieldId%quarter == 0)
	{
		pixelPosition.x += (layout.fieldHeight - layout.dividerOffset)/2;
		pixelPosition.y -= (layout.fieldHeight - layout.dividerOffset)/2;
	}
	else
	{
		if(fieldId%half > quarter)
		{
			pixelPosition.y -= (layout.fieldHeight - layout.dividerOffset)/2;
		}
		else
		{
			pixelPosition.x += (layout.fieldHeight - layout.dividerOffset)/2;
		}
	}

	return (Vector3){
		board.position.x + (pixelPosition.x / board.layout.boardSize * 2 - 1),
		board.position.y,
		board.position.z + (pixelPosition.y / board.layout.boardSize * 2 - 1),
	};
}
