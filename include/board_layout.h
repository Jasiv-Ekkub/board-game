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

	float modelScale;

	float boardSize;

	float fontSize;
	float fontSpacing;

	float nameOffsetEdge;
	float commentOffsetEdge;
	float imageSizeEdge;

	float nameOffsetCorner;
	float commentOffsetCorner;
	float imageSizeCorner;
} BoardLayout;

BoardLayout GetBoardLayout();
Vector3 CalculatePlayerPosition(Board board, int playerId);
float GetModelScale(Board board);

#endif //BOARD_LAYOUT_H
