#include <board_layout.h>

BoardLayout GetBoardLayout()
{
	return (BoardLayout){
	.fieldWidth = 300,
	.fieldHeight = 450,
	
	.borderWidth = 8,
	.dividerOffset = 100,
	.pawnOffset = 30,

	.modelScale = 0.05f,

	.fontSize = 48,
	.fontSpacing = 0,

	.nameOffsetEdge = 140,
	.commentOffsetEdge = 200,
	.imageSizeEdge = 200,

	.nameOffsetCorner = 105,
	.commentOffsetCorner = 150,
	.imageSizeCorner = 320,
	};
}
