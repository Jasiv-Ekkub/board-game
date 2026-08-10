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


void ImageDrawTextSpec(Image* image, Font font, const char* text, Vector2 position, int rotation, float fontSize, float spacing, Color tint)
{
	Image textImage = ImageTextEx(font, text, fontSize, spacing, tint);

	ImageRotate(&textImage, rotation);

	ImageDrawImagePro(image, textImage,
		(Rectangle){0, 0, (float)textImage.width, (float)textImage.height},
		(Rectangle){
			position.x - (float)textImage.width/2,
			position.y - (float)textImage.height/2,
			(float)(textImage.width),
			(float)(textImage.height)},
		(Vector2){0}, 0,
		WHITE);

	UnloadImage(textImage);
}



void DrawBoardCorner(Image* image, Board board, Rectangle rectangle, int id)
{
	Field field = board.fields[id];
	BoardLayout layout = board.layout;

	char commentBuffer[FIELD_TEXT_LENGTH];
	int commentOffset;
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
			commentOffset = layout.commentOffsetCorner;
			break;
		case SUPERACTION:
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
			commentOffset = -layout.nameOffsetCorner;
			break;
	}
	ImageDrawTextSpec(
		image,
		board.assets.font,
		field.name,
		(Vector2){
			rectangle.x + rectangle.width/2 - layout.nameOffsetCorner,
			rectangle.y + rectangle.height/2 + layout.nameOffsetCorner,
		},
		45,
		layout.fontSize,
		layout.fontSpacing,
		board.assets.colors.dark
	);
	ImageDrawTextSpec(
		image,
		board.assets.font,
		commentBuffer,
		(Vector2){
			rectangle.x + rectangle.width/2 - commentOffset,
			rectangle.y + rectangle.height/2 + commentOffset,
		},
		45,
		layout.fontSize,
		layout.fontSpacing,
		board.assets.colors.dark
	);
}

void DrawBoardEdge(Image* image, Board board, Rectangle rectangle, int id)
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
			float offset = layout.dividerOffset + layout.borderWidth;
			ImageDrawRectangleRec(image, (Rectangle){
				rectangle.x,
				rectangle.y + offset,
				rectangle.width,
				rectangle.height - offset,
			}, board.assets.colors.light);
			snprintf(commentBuffer, FIELD_TEXT_LENGTH, "$%i", field.value);
			break;
		case SUPERACTION:
		case ACTION:
			ImageDrawRectangleRec(image, rectangle, board.assets.colors.light);

			Image icon = board.assets.images[field.imageId];
			ImageDrawImagePro(image, icon,
				(Rectangle){0, 0, (float)icon.width, (float)icon.height},
				(Rectangle){
					rectangle.x + (rectangle.width - layout.imageSizeEdge)/2,
					rectangle.y + (rectangle.height - layout.imageSizeEdge)/2,
					layout.imageSizeEdge,
					layout.imageSizeEdge,
				},
				(Vector2){0}, 0,
				WHITE
			);
			
			strncpy(commentBuffer, field.comment, FIELD_TEXT_LENGTH);
			break;
	}
	ImageDrawTextSpec(
		image,
		board.assets.font,
		field.name,
		(Vector2){
			rectangle.x + rectangle.width/2,
			rectangle.y + rectangle.height/2 + layout.nameOffsetEdge,
		},
		0,
		layout.fontSize,
		layout.fontSpacing,
		board.assets.colors.dark
	);
	ImageDrawTextSpec(
		image,
		board.assets.font,
		commentBuffer,
		(Vector2){
			rectangle.x + rectangle.width/2,
			rectangle.y + rectangle.height/2 + layout.commentOffsetEdge,
		},
		0,
		layout.fontSize,
		layout.fontSpacing,
		board.assets.colors.dark
	);
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
		DrawBoardEdge(&image, layout, renderData, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth,
			layout.fieldWidth,
			layout.fieldHeight,
			}, board->fields[quarter + i]);
		
		DrawBoardEdge(&image, layout, renderData, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth + sideOffset,
			layout.fieldWidth,
			layout.fieldHeight,
			}, board->fields[4*quarter - i]);
	}
	ImageRotateCCW(&image);
	for(int i=1; i<quarter; ++i)
	{
		DrawBoardEdge(&image, layout, renderData, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth,
			layout.fieldWidth,
			layout.fieldHeight,
			}, board->fields[2*quarter + i]);
		
		DrawBoardEdge(&image, layout, renderData, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth + sideOffset,
			layout.fieldWidth,
			layout.fieldHeight,
			}, board->fields[quarter - i]);
	}
	ImageRotateCW(&image);
	ImageDrawRectangleRec(&image, (Rectangle){cornerOffset, cornerOffset, sideOffset-cornerOffset, sideOffset-cornerOffset}, renderData.colorPalette.light);
	ImageDrawTextSpec(&image, renderData.font, "Board game", (Vector2){boardSize/2, boardSize/2}, 45, 120, 1, renderData.colorPalette.dark);

	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);
}
*/
