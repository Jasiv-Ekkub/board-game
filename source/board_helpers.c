#include <board_helpers.h>
#include <stdio.h>

Color colors[3] = {RED, GREEN, BLUE};

void LoadFieldsDefault(Board* board)
{
	const uint16_t fieldCount = board->fieldCount;
	const uint16_t quarter = fieldCount / 4;
	
	for(uint16_t i=0; i<fieldCount; ++i)
	{

		board->fields[i] = (Field){
			.value = 0,
			.buildingLevel = 0,
			.ownerId = 0,
		};


		uint16_t qi = i % quarter;
		if(qi == quarter / 2)
		{
			snprintf(board->fields[i].name, FIELD_NAME_LENGTH, "Chance");
			board->fields[i].color = GRAY;
		}
		else
		{
			snprintf(board->fields[i].name, FIELD_NAME_LENGTH, "Wasteland %i", i);
			board->fields[i].color = colors[i%3];
		}

	}
}

void LoadGraphicsDefault(Board* board)
{
	const int fontSize = 30;
	board->graphics = (BoardGraphics){
		.borderWidth = 6,
		.dividerOffset = 40,
		.fieldWidth = 200,
		.cornerSize = 350,

		.font = LoadFontEx("resource/font/Cabal.ttf", 120, 0, 0),
		.fontSize = fontSize,
		.fontSpacing = 1,

		.nameOffset = 70,
		.commentOffset = 30,

		.colorPalette = {WHITE, GRAY, BLACK},

		.boardTexture = {0},
	};
}

void DrawBoardCorner(Image* image, BoardGraphics graphics, const char* name, Rectangle rectangle);
void DrawBoardEdge(Image* image, BoardGraphics graphics, Rectangle rectangle, Field field);

void ImageDrawTextSpec(Image* image, Font font, const char* text, Vector2 position, float rotation, float fontSize, float spacing, Color tint)
{
	Image textImage = ImageTextEx(font, text, fontSize, spacing, tint);
	
	ImageRotate(&textImage, rotation);

	ImageDrawImagePro(image, textImage,
		(Rectangle){0, 0, textImage.width, textImage.height},
		(Rectangle){position.x - textImage.width/2, position.y - textImage.height/2, textImage.width, textImage.height},
		(Vector2){0}, 0,
		WHITE);
	
	UnloadImage(textImage);
}

Texture2D LoadBoardTexture(Board board)
{
	uint16_t edgeLength = board.fieldCount/4;
	const BoardGraphics graphics = board.graphics;

	uint64_t cornerOffset = graphics.cornerSize + graphics.borderWidth * 2;
	uint64_t edgeOffset = graphics.fieldWidth + graphics.borderWidth;

	uint64_t sideOffset = cornerOffset + edgeOffset * edgeLength - graphics.borderWidth;
	uint64_t pixelSize = sideOffset + cornerOffset;
	
	Image image = GenImageColor(pixelSize, pixelSize, graphics.colorPalette.dark);

	//Start
	DrawBoardCorner(&image, graphics, "Start", (Rectangle){
		graphics.borderWidth,
		graphics.borderWidth + sideOffset,
		graphics.cornerSize,
		graphics.cornerSize,
		});
	//Prison
	DrawBoardCorner(&image, graphics, "Prison", (Rectangle){
		graphics.borderWidth,
		graphics.borderWidth,
		graphics.cornerSize,
		graphics.cornerSize,
		});
	//Free parking
	DrawBoardCorner(&image, graphics, "Free parking", (Rectangle){
		graphics.borderWidth + sideOffset,
		graphics.borderWidth,
		graphics.cornerSize,
		graphics.cornerSize,
		});
	//Policeman
	DrawBoardCorner(&image, graphics, "Policeman", (Rectangle){
		graphics.borderWidth + sideOffset,
		graphics.borderWidth + sideOffset,
		graphics.cornerSize,
		graphics.cornerSize,
		});

	for(uint16_t i=0; i<edgeLength; ++i)
	{
		DrawBoardEdge(&image, graphics, (Rectangle){
			cornerOffset + i * edgeOffset,
			graphics.borderWidth,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board.fields[edgeLength + i]);
		
		DrawBoardEdge(&image, graphics, (Rectangle){
			cornerOffset + i * edgeOffset,
			graphics.borderWidth + sideOffset,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board.fields[4*edgeLength - i - 1]);
	}
	ImageRotateCCW(&image);
	for(uint16_t i=0; i<edgeLength; ++i)
	{
		DrawBoardEdge(&image, graphics, (Rectangle){
			cornerOffset + i * edgeOffset,
			graphics.borderWidth,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board.fields[2*edgeLength + i]);
		
		DrawBoardEdge(&image, graphics, (Rectangle){
			cornerOffset + i * edgeOffset,
			graphics.borderWidth + sideOffset,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board.fields[edgeLength - i - 1]);
	}
	ImageRotateCW(&image);

	ImageDrawRectangleRec(&image, (Rectangle){cornerOffset, cornerOffset, sideOffset-cornerOffset, sideOffset-cornerOffset}, graphics.colorPalette.light);
	ImageDrawTextSpec(&image, graphics.font, "SASALELE", (Vector2){pixelSize/2, pixelSize/2}, 45, 120, 1, graphics.colorPalette.dark);

	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);
	GenTextureMipmaps(&texture);
	return texture;
}

void DrawBoardCorner(Image* image, BoardGraphics graphics, const char* name, Rectangle rectangle)
{
	ImageDrawRectangleRec(image, rectangle, graphics.colorPalette.light);
	
	ImageDrawTextSpec(
		image,
		graphics.font,
		name,
		(Vector2){rectangle.x + rectangle.width/2, rectangle.y + rectangle.height/2},
		45,
		graphics.fontSize,
		graphics.fontSpacing,
		graphics.colorPalette.dark);
}

void DrawBoardEdge(Image* image, BoardGraphics graphics, Rectangle rectangle, Field field)
{
	ImageDrawRectangleRec(image, (Rectangle){
		rectangle.x,
		rectangle.y,
		rectangle.width,
		graphics.dividerOffset,
		}, field.color);

	uint64_t offset = graphics.dividerOffset + graphics.borderWidth;

	ImageDrawRectangleRec(image, (Rectangle){
		rectangle.x,
		rectangle.y + offset,
		rectangle.width,
		rectangle.height - offset,
		}, graphics.colorPalette.light);

	ImageDrawTextSpec(
		image,
		graphics.font,
		field.name,
		(Vector2){rectangle.x + rectangle.width/2, rectangle.y + rectangle.height - graphics.nameOffset},
		0,
		graphics.fontSize,
		graphics.fontSpacing,
		BLACK);

	ImageDrawTextSpec(
		image,
		graphics.font,
		"Placeholder",
		(Vector2){rectangle.x + rectangle.width/2, rectangle.y + rectangle.height - graphics.commentOffset},
		0,
		graphics.fontSize,
		graphics.fontSpacing,
		BLACK);
}

