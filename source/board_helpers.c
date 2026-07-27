#include <board_helpers.h>
#include <stdio.h>
#include <string.h>

Color colors[3] = {RED, GREEN, BLUE};

void LoadFieldsDefault(Board* board)
{
	const uint16_t fieldCount = board->fieldCount;
	const uint16_t quarter = fieldCount / 4;
	
	for(uint16_t i=0; i<fieldCount; ++i)
	{

		uint16_t qi = i % quarter;
		if(qi == quarter / 2)
		{
			board->fields[i] = (Field){
				.name = "Chance",
				.type = CHANCE,
				.comment = "Draw a card",
				.color = GRAY,
			};
		}
		else
		{
			board->fields[i] = (Field){
				.value = 0,
				.type = PROPERTY,
				.buildingLevel = 0,
				.ownerId = 0,
				.color = colors[i%3],
			};

			snprintf(board->fields[i].name, FIELD_NAME_LENGTH, "Land %i", i);
		}

	}
}

BoardGraphics LoadGraphics()
{
	printf("GAMEINFO: Load graphics data\n");
	return (BoardGraphics){
		.borderWidth = 6,
		.dividerOffset = 40,
		.fieldWidth = 250,
		.cornerSize = 400,
	};
}

BoardRenderData LoadRenderData()
{
	printf("GAMEINFO: Loaded render data\n");
	BoardRenderData renderData = {

		.font = LoadFontEx("resource/font/Cabal.ttf", 120, 0, 0),
		.fontSize = 36,
		.fontSpacing = 0,

		.nameOffset = 70,
		.commentOffset = 30,

		.colorPalette = {WHITE, BLACK},
		
		.cornerImageScale = 250,

		.cornerImages = {
			GenImageChecked(500, 500, 2, 2, RED, BLUE),
			GenImageChecked(500, 500, 2, 2, GREEN, BLUE),
			GenImageChecked(500, 500, 2, 2, RED, GREEN),
			GenImageChecked(500, 500, 2, 2, RED, PINK),
		}
	};
	
	for(int i=4; i--;) ImageRotate(&renderData.cornerImages[i], 45);
	
	return renderData;
}

void UnloadRenderData(BoardRenderData renderData)
{
	printf("GAMEINFO: Unloaded render data\n");
	UnloadFont(renderData.font);

	for(int i=4; i--;) UnloadImage(renderData.cornerImages[i]);
}

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

void DrawBoardCorner(Image* image, BoardGraphics graphics, BoardRenderData renderData, uint8_t cornerImageId, const char* name, Rectangle rectangle)
{
	ImageDrawRectangleRec(image, rectangle, renderData.colorPalette.light);

	Image* cornerImage = &renderData.cornerImages[cornerImageId];
	ImageDrawImagePro(
		image, renderData.cornerImages[cornerImageId],
		(Rectangle){0, 0, cornerImage->width, cornerImage->height},
		(Rectangle){
			rectangle.x + (rectangle.width - renderData.cornerImageScale)/2,
			rectangle.y + (rectangle.height - renderData.cornerImageScale)/2,
			renderData.cornerImageScale,
			renderData.cornerImageScale,
		}, (Vector2){0},
		45, WHITE);	

	ImageDrawTextSpec(
		image,
		renderData.font,
		name,
		(Vector2){rectangle.x + rectangle.width/4, rectangle.y + 3*rectangle.height/4},
		45,
		renderData.fontSize,
		renderData.fontSpacing,
		renderData.colorPalette.dark);
}

void DrawBoardEdge(Image* image, BoardGraphics graphics, BoardRenderData renderData, Rectangle rectangle, Field field)
{
	char buffer[FIELD_NAME_LENGTH] = {0};
	switch(field.type)
	{
		case PROPERTY:
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
				}, renderData.colorPalette.light);

			snprintf(buffer, FIELD_NAME_LENGTH, "$%i", field.value);
			break;
		default:
			ImageDrawRectangleRec(image, (Rectangle){
				rectangle.x,
				rectangle.y,
				rectangle.width,
				rectangle.height,
				}, renderData.colorPalette.light);
			strncpy(buffer, field.comment, FIELD_NAME_LENGTH);
			break;
	};

	ImageDrawTextSpec(
		image,
		renderData.font,
		field.name,
		(Vector2){rectangle.x + rectangle.width/2, rectangle.y + rectangle.height - renderData.nameOffset},
		0,
		renderData.fontSize,
		renderData.fontSpacing,
		renderData.colorPalette.dark);
	

	ImageDrawTextSpec(
		image,
		renderData.font,
		buffer,
		(Vector2){rectangle.x + rectangle.width/2, rectangle.y + rectangle.height - renderData.commentOffset},
		0,
		renderData.fontSize,
		renderData.fontSpacing,
		renderData.colorPalette.dark);
}

Vector2 CalculateFieldCenter(Board board, uint16_t fieldNumber)
{
	BoardGraphics graphics = board.graphics;
	uint16_t quarter = board.fieldCount/4 + 1;
	uint64_t offsetCorner = graphics.borderWidth + graphics.cornerSize/2;
	
	Vector2 position = {0};

	uint16_t sideId = fieldNumber/quarter;
	uint16_t sideFieldId = fieldNumber%quarter;
	if(sideFieldId == 0)
	{
		switch(sideId)
		{
			case 0:
				position = (Vector2){
					offsetCorner,
					graphics.pixelSize - offsetCorner,
				};
				break;
			case 1:
				position = (Vector2){
					offsetCorner,
					offsetCorner,
				};
				break;
			case 2:
				position = (Vector2){
					graphics.pixelSize - offsetCorner,
					offsetCorner,
				};
				break;
			case 3:
				position = (Vector2){
					graphics.pixelSize - offsetCorner,
					graphics.pixelSize - offsetCorner,
				};
				break;
		}
	}
	else
	{
		uint64_t offsetEdgeBase = offsetCorner*2 + graphics.fieldWidth/2;
		uint64_t offsetEdge = graphics.fieldWidth + graphics.borderWidth;

		uint64_t edgeOffsetFinal =  offsetEdgeBase + offsetEdge*(sideFieldId - 1);
		switch(sideId)
		{
			case 0:
				position = (Vector2){
					offsetCorner,
					graphics.pixelSize - edgeOffsetFinal,
				};
				break;
			case 1:
				position = (Vector2){
					edgeOffsetFinal,
					offsetCorner,
				};
				break;
			case 2:
				position = (Vector2){
					graphics.pixelSize - offsetCorner,
					edgeOffsetFinal,
				};
				break;
			case 3:
				position = (Vector2){
					graphics.pixelSize - edgeOffsetFinal,
					graphics.pixelSize - offsetCorner,
				};
				break;
		}
	}

	Vector2 finalPosition = {
		(float)position.x / graphics.pixelSize * 2 - 1,
		(float)position.y / graphics.pixelSize * 2 - 1,
	};

	return finalPosition;
}
