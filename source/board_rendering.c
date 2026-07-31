#include <board_rendering.h>
#include <stdio.h>
#include <string.h>

Image LoadImageDefault()
{
	return GenImageChecked(256,256,16,16,BLACK,PURPLE);
}

Texture2D LoadTextureDefault()
{
	Image image = LoadImageDefault();
	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);
	return texture;
}

void DrawBoardCorner(Image* image, Board board, Rectangle rectangle, int id)
{
	Field field = board.fields[id];
	BoardLayout layout = board.layout;

	char commentBuffer[FIELD_TEXT_LENGTH];
	switch(field.type)
	{
		case PROPERTY:
			ImageDrawRectangleRec(image, (Rectangle){
				rectangle.x,
				rectangle.y,
				rectangle.width,
				layout.dividerOffset,
			}, field.color);
			ImageDrawRectangleRec(image, (Rectangle){
				rectangle.x + rectangle.width - layout.dividerOffset,
				rectangle.y + layout.dividerOffset,
				layout.dividerOffset,
				rectangle.height - layout.dividerOffset,
			}, field.color);
			float offset = layout.dividerOffset + layout.borderWidth;
			ImageDrawRectangleRec(image, (Rectangle){
				rectangle.x,
				rectangle.y + offset,
				rectangle.width - offset,
				rectangle.height - offset,
			}, board.assets.colors.light);
			snprintf(commentBuffer, FIELD_TEXT_LENGTH, "$%i", field.value);
			break;
		case ACTION:
			ImageDrawRectangleRec(image, rectangle, board.assets.colors.light);

			Image icon = ImageCopy(board.assets.images[field.imageId]);
			ImageRotate(&icon, 45);
			ImageDrawImagePro(image, icon,
				(Rectangle){0, 0, (float)icon.width, (float)icon.height},
				(Rectangle){
					rectangle.x + (rectangle.width - layout.imageSizeCorner)/2,
					rectangle.y + (rectangle.height - layout.imageSizeCorner)/2,
					layout.imageSizeCorner,
					layout.imageSizeCorner,
				},
				(Vector2){0,0}, 45,
				WHITE
			);
			UnloadImage(icon);
			strncpy(commentBuffer, field.comment, FIELD_TEXT_LENGTH);
			break;
	}

}
/*
LoadBoardTexture(Board* board)
{
	BoardLayout layout = board->layout;
	BoardAssets assets = board->assets;

	int quarter = board->fieldCount/4;

	float cornerOffset = layout.fieldHeight + layout.borderWidth * 2;
	float edgeOffset = layout.fieldWidth + layout.borderWidth;

	float sideOffset = cornerOffset + edgeOffset * (float)(quarter-1) - layout.borderWidth;

	float boardSize = sideOffset + cornerOffset;
	board->layout.boardSize = boardSize;
	
	Image image = GenImageColor((int)boardSize, (int)boardSize, renderData.colorPalette.dark);


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
	ImageDrawTextSpec(&image, renderData.font, "Board game", (Vector2){boardSize/2, boardSize/2}, 45, 120, 1, renderData.colorPalette.dark);

	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);
}
*/
