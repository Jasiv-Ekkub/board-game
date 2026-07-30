#include <board.h>

Board LoadBoard(const char* filename, int size)
{
	if(size > MAX_FIELD_AMOUNT)
		size = MAX_FIELD_AMOUNT;

	else if(size < MIN_FIELD_AMOUNT)
		size = MIN_FIELD_AMOUNT;

	else size &= 0xFFFC;


	Board board = {
		.assets = LoadBoardAssets();
		.layout = GetBoardLayout();

		.fieldCount = size,
		.playerCount = 4,
		.players = {
			GetPlayer("Player A", 1000),
			GetPlayer("Player B", 1000),
			GetPlayer("Player C", 1000),
			GetPlayer("Player D", 1000),
		},
	};

	LoadFields(&board);
	return board;
}

void UpdateBoard(Board* board, GameContext gameContext)
{
}

void UnloadBoard(Board board)
{
	UnloadBoardAssets(board.assets);
}

Texture2D LoadBoardTexture(Board* board, BoardRenderData renderData)
{
	int quarter = board->fieldCount/4;
	const BoardGraphics graphics = board->graphics;

	float cornerOffset = graphics.cornerSize + graphics.borderWidth * 2;
	float edgeOffset = graphics.fieldWidth + graphics.borderWidth;

	float sideOffset = cornerOffset + edgeOffset * (float)(quarter-1) - graphics.borderWidth;

	float pixelSize = sideOffset + cornerOffset;
	board->graphics.pixelSize = pixelSize;
	
	Image image = GenImageColor((int)pixelSize, (int)pixelSize, renderData.colorPalette.dark);

	//Down
	DrawBoardCorner(&image, graphics, renderData, (Rectangle){
		graphics.borderWidth,
		graphics.borderWidth + sideOffset,
		graphics.cornerSize,
		graphics.cornerSize,
		}, board->fields[0]);
	//Left
	DrawBoardCorner(&image, graphics, renderData, (Rectangle){
		graphics.borderWidth,
		graphics.borderWidth,
		graphics.cornerSize,
		graphics.cornerSize,
		}, board->fields[quarter]);
	//Up
	DrawBoardCorner(&image, graphics, renderData, (Rectangle){
		graphics.borderWidth + sideOffset,
		graphics.borderWidth,
		graphics.cornerSize,
		graphics.cornerSize,
		}, board->fields[quarter * 2]);
	//Right
	DrawBoardCorner(&image, graphics, renderData, (Rectangle){
		graphics.borderWidth + sideOffset,
		graphics.borderWidth + sideOffset,
		graphics.cornerSize,
		graphics.cornerSize,
		}, board->fields[quarter * 3]);

	for(int i=1; i<quarter; ++i)
	{
		DrawBoardEdge(&image, graphics, renderData, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			graphics.borderWidth,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board->fields[quarter + i]);
		
		DrawBoardEdge(&image, graphics, renderData, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			graphics.borderWidth + sideOffset,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board->fields[4*quarter - i]);
	}
	ImageRotateCCW(&image);
	for(int i=1; i<quarter; ++i)
	{
		DrawBoardEdge(&image, graphics, renderData, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			graphics.borderWidth,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board->fields[2*quarter + i]);
		
		DrawBoardEdge(&image, graphics, renderData, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			graphics.borderWidth + sideOffset,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board->fields[quarter - i]);
	}
	ImageRotateCW(&image);

	ImageDrawRectangleRec(&image, (Rectangle){cornerOffset, cornerOffset, sideOffset-cornerOffset, sideOffset-cornerOffset}, renderData.colorPalette.light);
	ImageDrawTextSpec(&image, renderData.font, "Board game", (Vector2){pixelSize/2, pixelSize/2}, 45, 120, 1, renderData.colorPalette.dark);

	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);
	GenTextureMipmaps(&texture);
	return texture;
}
