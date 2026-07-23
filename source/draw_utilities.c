#include <draw_utilities.h>
#include <board_info.h>
#include <stdbool.h>

#define BORDER 16
#define DIVIDER 100
#define WIDTH 300
#define HEIGHT 500

#define FONT_SIZE 50 
#define FONT_SPACING 1

#define NAME_OFFSET 120
#define COMMENT_OFFSET 60

#define BOARD_LIGHT WHITE
#define BOARD_DARK BLACK 

Font font;

void ImageDrawTextCentered(Image* image, const char* text, Vector2 position, Color color)
{
	Vector2 origin = MeasureTextEx(font, text, FONT_SIZE, FONT_SPACING);
	origin.x /= 2;
	origin.y /= 2;

	position.x -= origin.x;
	position.y -= origin.y;

	ImageDrawTextEx(image, font, text, position, FONT_SIZE, FONT_SPACING, color);
}

Image LoadEdgeFieldBaseImage()
{
	Image fieldImage = GenImageColor(WIDTH, HEIGHT, BOARD_LIGHT);
	ImageDrawRectangle(&fieldImage, 0, 0, WIDTH, BORDER, BOARD_DARK);
	ImageDrawRectangle(&fieldImage, 0, HEIGHT - BORDER, WIDTH, BORDER, BOARD_DARK);
	ImageDrawRectangle(&fieldImage, 0, 0, BORDER, HEIGHT, BOARD_DARK);
	ImageDrawRectangle(&fieldImage, WIDTH - BORDER, 0, BORDER, HEIGHT, BOARD_DARK);

	return fieldImage;
}

Image LoadEdgeFieldImage(const char* name, const char* comment, Color color)
{
	Image fieldImage = LoadEdgeFieldBaseImage();
	
	ImageDrawRectangle(&fieldImage, BORDER, BORDER, WIDTH-2*BORDER, DIVIDER-BORDER, color);
	ImageDrawRectangle(&fieldImage, 0, DIVIDER, WIDTH, BORDER, BOARD_DARK);
	
	ImageDrawTextCentered(&fieldImage, name, (Vector2){WIDTH/2, HEIGHT - NAME_OFFSET}, BOARD_DARK);
	ImageDrawTextCentered(&fieldImage, comment, (Vector2){WIDTH/2, HEIGHT - COMMENT_OFFSET}, BOARD_DARK);

	return fieldImage;
}

Image LoadEdgeFieldRotatedImage(const char* name, const char* comment, Color color)
{
	Image fieldImage = LoadEdgeFieldImage(name, comment, color);
	ImageRotateCCW(&fieldImage);
	return fieldImage;
}

Image LoadCornerFieldBaseImage()
{
	Image fieldImage = GenImageColor(HEIGHT, HEIGHT, BOARD_LIGHT);
	ImageDrawRectangle(&fieldImage, 0, 0, HEIGHT , BORDER, BOARD_DARK);
	ImageDrawRectangle(&fieldImage, 0, HEIGHT - BORDER, HEIGHT , BORDER, BOARD_DARK);
	ImageDrawRectangle(&fieldImage, 0, 0, BORDER, HEIGHT, BOARD_DARK);
	ImageDrawRectangle(&fieldImage, HEIGHT - BORDER, 0, BORDER, HEIGHT, BOARD_DARK);

	return fieldImage;
}

void ImageDrawBoardFieldImage(Image* boardImage, FieldInfo fieldInfo, int x, int y, bool flipped)
{
	char buffer[20] = {0};
	GetFieldComment(buffer, 20, fieldInfo);

	Image fieldImage = LoadEdgeFieldImage(fieldInfo.name, buffer, fieldInfo.color);
	if(flipped) ImageRotate(&fieldImage, 90);
	ImageDrawImage(boardImage, fieldImage, x, y, WHITE);
	UnloadImage(fieldImage);

}

Texture2D LoadBoardTexture(int size)
{
	font = GetFontDefault();
	int pixelSize = size * WIDTH + 2 * HEIGHT;
	Image boardImage = GenImageColor(pixelSize, pixelSize, BLACK);
	ImageDrawRectangle(&boardImage, HEIGHT, HEIGHT, pixelSize-HEIGHT*2, pixelSize-HEIGHT*2, (Color){0});

	Image cornerFieldImage = LoadCornerFieldBaseImage();
	
	int sideOffset = pixelSize - HEIGHT;
	ImageDrawImage(&boardImage, cornerFieldImage, 0, 0, WHITE);
	ImageDrawImage(&boardImage, cornerFieldImage, sideOffset, 0, WHITE);
	ImageDrawImage(&boardImage, cornerFieldImage, 0, sideOffset, WHITE);
	ImageDrawImage(&boardImage, cornerFieldImage, sideOffset, sideOffset, WHITE);
	UnloadImage(cornerFieldImage);
	
	for(int i=0; i<size; ++i)
	{
		ImageDrawBoardFieldImage(&boardImage, fieldRegistry[i], HEIGHT + i * WIDTH, 0, false);
		ImageDrawBoardFieldImage(&boardImage, fieldRegistry[i + size], sideOffset, HEIGHT + i * WIDTH, true);
		ImageDrawBoardFieldImage(&boardImage, fieldRegistry[i + size*2], HEIGHT + (size - i - 1) * WIDTH, sideOffset, false);
		ImageDrawBoardFieldImage(&boardImage, fieldRegistry[i + size*3], 0 , HEIGHT + (size - i - 1) * WIDTH, true);
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
