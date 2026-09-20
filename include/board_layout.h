#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

#include <raylib.h>

typedef struct Board Board;

typedef struct BoardLayout
{
	float fieldWidth;
	float fieldHeight;

	float borderWidth;
	float dividerOffset;
	float pawnOffset;

	float modelScaleMultiplier;
	float modelScale;
	
	float cameraFovMultiplier;

	float boardSize;

	float fontSize;
	float fontSpacing;

	float nameOffsetEdge;
	float commentOffsetEdge;
	float imageSizeEdge;
	float imageOffsetEdge;

	float nameOffsetCorner;
	float commentOffsetCorner;
	float imageSizeCorner;
} BoardLayout;

BoardLayout GetBoardLayout();
Vector3 CalculatePlayerPosition(Board board, int playerId, int playerPosition);
Vector3 CalculateHousePosition(Board board, int fieldId);
float GetModelScale(Board board);

#endif //BOARD_LAYOUT_H
