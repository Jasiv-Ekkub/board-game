#include <draw_utilities.h>

#define BORDER 8
#define DIVIDER 50
#define WIDTH 150
#define HEIGHT 250

#define FONT_SIZE 20 
#define FONT_SPACING 1

#define NAME_OFFSET 60
#define COMMENT_OFFSET 30

#define BOARD_LIGHT WHITE
#define BOARD_DARK BLACK 

void ImageDrawTextCentered(Image* image, const char* text, Vector2 position, Color color)
{
	Font font = GetFontDefault();
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
	ImageRotate(&fieldImage, 90);
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

Texture2D LoadBoardTexture(int size)
{
	int pixelSize = size * WIDTH + 2 * HEIGHT;
	Image boardImage = GenImageColor(pixelSize, pixelSize, (Color){0,0,0,0});

	Image cornerFieldImage = LoadCornerFieldBaseImage();
	
	int sideOffset = pixelSize - HEIGHT;
	ImageDrawImage(&boardImage, cornerFieldImage, 0, 0, WHITE);
	ImageDrawImage(&boardImage, cornerFieldImage, sideOffset, 0, WHITE);
	ImageDrawImage(&boardImage, cornerFieldImage, 0, sideOffset, WHITE);
	ImageDrawImage(&boardImage, cornerFieldImage, sideOffset, sideOffset, WHITE);
	UnloadImage(cornerFieldImage);

	Image edgeFieldImage = LoadEdgeFieldImage("Warszawa", "-$100", RED);
	Image edgeFieldRotatedImage = LoadEdgeFieldRotatedImage("Wadowice", "$2137", YELLOW);
	for(int i=0; i<size; ++i)
	{
		ImageDrawImage(&boardImage, edgeFieldImage, i*WIDTH + HEIGHT, 0, WHITE);
		ImageDrawImage(&boardImage, edgeFieldImage, i*WIDTH + HEIGHT, sideOffset, WHITE);

		ImageDrawImage(&boardImage, edgeFieldRotatedImage, 0, i*WIDTH + HEIGHT, WHITE);
		ImageDrawImage(&boardImage, edgeFieldRotatedImage, sideOffset, i*WIDTH + HEIGHT, WHITE);
	}

	Texture2D boardTexture = LoadTextureFromImage(boardImage);
	UnloadImage(boardImage);
	return boardTexture;
}
