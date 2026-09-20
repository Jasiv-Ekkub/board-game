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

Texture2D LoadTextureDice()
{
	const int sideRes = 256;
	const int borderSize = 12;
	const int dotSize = 20;
	const int sideResHalf = sideRes/2;
	Image image = GenImageColor(3*sideRes, 2*sideRes, WHITE);

	ImageDrawRectangle(&image, 0, 0, 3*sideRes, borderSize, RED);
	ImageDrawRectangle(&image, 0, sideRes - borderSize, 3*sideRes, 2*borderSize, RED);
	ImageDrawRectangle(&image, 0, 2*sideRes - borderSize, 3*sideRes, borderSize, RED);

	ImageDrawRectangle(&image, 0, 0, borderSize, 2*sideRes, RED);
	ImageDrawRectangle(&image, sideRes - borderSize, 0, 2*borderSize, 2*sideRes, RED);
	ImageDrawRectangle(&image, 2*sideRes - borderSize, 0, 2*borderSize, 2*sideRes, RED);
	ImageDrawRectangle(&image, 3*sideRes - borderSize, 0, borderSize, 2*sideRes, RED);

	//1
	ImageDrawCircle(&image, sideResHalf, sideResHalf, dotSize, RED);
	//2
	ImageDrawCircle(&image, sideRes + sideResHalf/2, sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, 2*sideRes - sideResHalf/2, sideRes - sideResHalf/2, dotSize, RED);
	//3
	ImageDrawCircle(&image, sideResHalf + 2*sideRes, sideResHalf, dotSize, RED);
	ImageDrawCircle(&image, 2*sideRes + sideResHalf/2, sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, 3*sideRes - sideResHalf/2, sideRes - sideResHalf/2, dotSize, RED);
	//4
	ImageDrawCircle(&image, sideResHalf/2, 2*sideRes - sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, sideRes - sideResHalf/2, 2*sideRes - sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, sideResHalf/2, sideRes + sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, sideRes - sideResHalf/2, sideRes + sideResHalf/2, dotSize, RED);
	//5
	ImageDrawCircle(&image, sideResHalf + sideRes, sideResHalf + sideRes, dotSize, RED);
	ImageDrawCircle(&image, sideRes + sideResHalf/2, 2*sideRes - sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, 2*sideRes - sideResHalf/2, 2*sideRes - sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, sideRes + sideResHalf/2, sideRes + sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, 2*sideRes - sideResHalf/2, sideRes + sideResHalf/2, dotSize, RED);
	//6
	ImageDrawCircle(&image, 2*sideRes + sideResHalf/2, 2*sideRes - sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, 3*sideRes - sideResHalf/2, 2*sideRes - sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, 2*sideRes + sideResHalf/2, sideRes + sideResHalf, dotSize, RED);
	ImageDrawCircle(&image, 3*sideRes - sideResHalf/2, sideRes + sideResHalf, dotSize, RED);
	ImageDrawCircle(&image, 2*sideRes + sideResHalf/2, sideRes + sideResHalf/2, dotSize, RED);
	ImageDrawCircle(&image, 3*sideRes - sideResHalf/2, sideRes + sideResHalf/2, dotSize, RED);

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
			}, board.colors.light);
			snprintf(commentBuffer, FIELD_TEXT_LENGTH, "$%i", field.value);
			commentOffset = layout.commentOffsetCorner;
			break;
		case SUPERACTION:
		case ACTION:
			ImageDrawRectangleRec(image, rectangle, board.colors.light);

			Image icon = ImageCopy(images[field.imageId]);
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
		font,
		field.name,
		(Vector2){
			rectangle.x + rectangle.width/2 - layout.nameOffsetCorner,
			rectangle.y + rectangle.height/2 + layout.nameOffsetCorner,
		},
		45,
		layout.fontSize,
		layout.fontSpacing,
		board.colors.dark
	);
	ImageDrawTextSpec(
		image,
		font,
		commentBuffer,
		(Vector2){
			rectangle.x + rectangle.width/2 - commentOffset,
			rectangle.y + rectangle.height/2 + commentOffset,
		},
		45,
		layout.fontSize,
		layout.fontSpacing,
		board.colors.dark
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
			}, board.colors.light);
			snprintf(commentBuffer, FIELD_TEXT_LENGTH, "$%i", field.value);
			break;
		case SUPERACTION:
		case ACTION:
			ImageDrawRectangleRec(image, rectangle, board.colors.light);

			Image icon = images[field.imageId];
			ImageDrawImagePro(image, icon,
				(Rectangle){0, 0, (float)icon.width, (float)icon.height},
				(Rectangle){
					rectangle.x + (rectangle.width - layout.imageSizeEdge)/2,
					rectangle.y + (rectangle.height - layout.imageSizeEdge)/2 + layout.imageOffsetEdge,
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
		font,
		field.name,
		(Vector2){
			rectangle.x + rectangle.width/2,
			rectangle.y + rectangle.height/2 + layout.nameOffsetEdge,
		},
		0,
		layout.fontSize,
		layout.fontSpacing,
		board.colors.dark
	);
	ImageDrawTextSpec(
		image,
		font,
		commentBuffer,
		(Vector2){
			rectangle.x + rectangle.width/2,
			rectangle.y + rectangle.height/2 + layout.commentOffsetEdge,
		},
		0,
		layout.fontSize,
		layout.fontSpacing,
		board.colors.dark
	);
}
