#include <board_layout.h>

BoardLayout GetBoardLayout()
{
	return (BoardLayout){
	.fieldWidth = 100,
	.fieldHeight = 220,
	
	.borderWidth = 8,
	.dividerOffset = 40,
	.pawnOffset = 30,

	.modelScale = 0.05f,

	.fontSize = 24,
	.fontSpacing = 1,

	.nameOffsetEdge = 60,
	.commentOffsetEdge = 30,
	.imageSizeEdge = 80,

	.nameOffsetCorner = 60,
	.commentOffsetCorner = 30,
	.imageSizeCorner = 180,
	};
}
