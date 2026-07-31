#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

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

#endif //BOARD_LAYOUT_H
