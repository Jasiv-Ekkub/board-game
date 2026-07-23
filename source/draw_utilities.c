#include <draw_utilities.h>
#include <board_info.h>
#include <stdbool.h>

Image LoadCornerFieldBaseImage();
void ImageDrawBoardFieldImage(Image* boardImage, FieldInfo fieldInfo, int x, int y, bool flipped);

Font font;

Texture2D LoadBoardTexture(int size)
{
	font = GetFontDefault();
	int pixelSize = size * boardInfo.graphicalInfo.width + 2 * boardInfo.graphicalInfo.height;
	Image boardImage = GenImageColor(pixelSize, pixelSize, BLACK);
	ImageDrawRectangle(&boardImage, boardInfo.graphicalInfo.height, boardInfo.graphicalInfo.height, pixelSize-boardInfo.graphicalInfo.height*2, pixelSize-boardInfo.graphicalInfo.height*2, (Color){0});

	Image cornerFieldImage = LoadCornerFieldBaseImage();
	
	int sideOffset = pixelSize - boardInfo.graphicalInfo.height;
	ImageDrawImage(&boardImage, cornerFieldImage, 0, 0, WHITE);
	ImageDrawImage(&boardImage, cornerFieldImage, sideOffset, 0, WHITE);
	ImageDrawImage(&boardImage, cornerFieldImage, 0, sideOffset, WHITE);
	ImageDrawImage(&boardImage, cornerFieldImage, sideOffset, sideOffset, WHITE);
	UnloadImage(cornerFieldImage);
	
	for(int i=0; i<size; ++i)
	{
		ImageDrawBoardFieldImage(&boardImage, boardInfo.fieldRegistry[i], boardInfo.graphicalInfo.height + i * boardInfo.graphicalInfo.width, 0, false);
		ImageDrawBoardFieldImage(&boardImage, boardInfo.fieldRegistry[i + size], sideOffset, boardInfo.graphicalInfo.height + i * boardInfo.graphicalInfo.width, true);
		ImageDrawBoardFieldImage(&boardImage, boardInfo.fieldRegistry[i + size*2], boardInfo.graphicalInfo.height + (size - i - 1) * boardInfo.graphicalInfo.width, sideOffset, false);
		ImageDrawBoardFieldImage(&boardImage, boardInfo.fieldRegistry[i + size*3], 0 , boardInfo.graphicalInfo.height + (size - i - 1) * boardInfo.graphicalInfo.width, true);
	}
	
	Texture2D boardTexture = LoadTextureFromImage(boardImage);
	GenTextureMipmaps(&boardTexture);
	UnloadImage(boardImage);
	return boardTexture;
}

Model LoadBoardModel(Texture2D texture, int size)
{
	Model boardModel = LoadModelFromMesh(GenMeshPlane(size, size, 4, 4));
	boardModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	return boardModel;
}


//---
void ImageDrawTextCentered(Image* image, const char* text, Vector2 position, Color color)
{
	Vector2 origin = MeasureTextEx(font, text, boardInfo.graphicalInfo.fontSize, boardInfo.graphicalInfo.fontSpacing);
	origin.x /= 2;
	origin.y /= 2;

	position.x -= origin.x;
	position.y -= origin.y;

	ImageDrawTextEx(image, font, text, position, boardInfo.graphicalInfo.fontSize, boardInfo.graphicalInfo.fontSpacing, color);
}


//---
Image LoadEdgeFieldBaseImage()
{
	Image fieldImage = GenImageColor(boardInfo.graphicalInfo.width, boardInfo.graphicalInfo.height, boardInfo.graphicalInfo.boardLight);
	ImageDrawRectangle(&fieldImage, 0, 0, boardInfo.graphicalInfo.width, boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.boardDark);
	ImageDrawRectangle(&fieldImage, 0, boardInfo.graphicalInfo.height - boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.width, boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.boardDark);
	ImageDrawRectangle(&fieldImage, 0, 0, boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.height, boardInfo.graphicalInfo.boardDark);
	ImageDrawRectangle(&fieldImage, boardInfo.graphicalInfo.width - boardInfo.graphicalInfo.border, 0, boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.height, boardInfo.graphicalInfo.boardDark);

	return fieldImage;
}

Image LoadEdgeFieldImage(const char* name, const char* comment, Color color)
{
	Image fieldImage = LoadEdgeFieldBaseImage();
	
	ImageDrawRectangle(&fieldImage, boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.width-2*boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.divider-boardInfo.graphicalInfo.border, color);
	ImageDrawRectangle(&fieldImage, 0, boardInfo.graphicalInfo.divider, boardInfo.graphicalInfo.width, boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.boardDark);
	
	ImageDrawTextCentered(&fieldImage, name, (Vector2){boardInfo.graphicalInfo.width/2, boardInfo.graphicalInfo.height - boardInfo.graphicalInfo.nameOffset}, boardInfo.graphicalInfo.boardDark);
	ImageDrawTextCentered(&fieldImage, comment, (Vector2){boardInfo.graphicalInfo.width/2, boardInfo.graphicalInfo.height - boardInfo.graphicalInfo.commentOffset}, boardInfo.graphicalInfo.boardDark);

	return fieldImage;
}


//---
Image LoadCornerFieldBaseImage()
{
	Image fieldImage = GenImageColor(boardInfo.graphicalInfo.height, boardInfo.graphicalInfo.height, boardInfo.graphicalInfo.boardLight);
	ImageDrawRectangle(&fieldImage, 0, 0, boardInfo.graphicalInfo.height , boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.boardDark);
	ImageDrawRectangle(&fieldImage, 0, boardInfo.graphicalInfo.height - boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.height , boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.boardDark);
	ImageDrawRectangle(&fieldImage, 0, 0, boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.height, boardInfo.graphicalInfo.boardDark);
	ImageDrawRectangle(&fieldImage, boardInfo.graphicalInfo.height - boardInfo.graphicalInfo.border, 0, boardInfo.graphicalInfo.border, boardInfo.graphicalInfo.height, boardInfo.graphicalInfo.boardDark);

	return fieldImage;
}


//---
void ImageDrawBoardFieldImage(Image* boardImage, FieldInfo fieldInfo, int x, int y, bool flipped)
{
	char buffer[20] = {0};
	GetFieldComment(buffer, 20, fieldInfo);

	Image fieldImage = LoadEdgeFieldImage(fieldInfo.name, buffer, fieldInfo.color);
	if(flipped) ImageRotate(&fieldImage, 90);
	ImageDrawImage(boardImage, fieldImage, x, y, WHITE);
	UnloadImage(fieldImage);

}
