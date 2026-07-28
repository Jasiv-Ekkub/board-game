#include <board_helpers.h>
#include <stdio.h>
#include <string.h>

Color colors[6] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};

void LoadFieldsDefault(Board* board)
{
	const uint16_t fieldCount = board->fieldCount;
	const uint16_t quarter = fieldCount / 4;
	
	for(uint16_t i=0; i<fieldCount; ++i)
	{
		uint16_t qi = i%quarter;
		if(i == 0)
		{
			board->fields[i] = (Field){
				.name = "Corner",
				.type = ACTION,
				.imageId = START_IMAGE,
				.comment = "Test",
			};
		}
		else if(qi == quarter/2)
		{
			board->fields[i] = (Field){
				.name = "CHANCE",
				.type = ACTION,
				.imageId = CHANCE_IMAGE,
				.comment = "Draw a card",
			};
		}
		else
		{
			board->fields[i] = (Field){
				.value = 0,
				.type = PROPERTY,
				.buildingLevel = 0,
				.ownerId = 0,
				.color = colors[i%6],
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
		.dividerOffset = 80,
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

		.nameOffsetEdge = 70,
		.commentOffsetEdge = 30,
		.imageSizeEdge = 150,

		.nameOffsetCorner = 80,
		.commentOffsetCorner = 50,
		.imageSizeCorner = 150,

		.colorPalette = {WHITE, BLACK},
		
		.images = {
			GenImageChecked(4, 4, 1, 1, RED, BLUE),
			GenImageChecked(4, 4, 1, 1, GREEN, BLUE),
			GenImageChecked(4, 4, 1, 1, RED, GREEN),
			GenImageChecked(4, 4, 1, 1, RED, PINK),
			GenImageChecked(4, 4, 1, 1, YELLOW, PINK),
		}
	};
	
	return renderData;
}

void UnloadRenderData(BoardRenderData renderData)
{
	printf("GAMEINFO: Unloaded render data\n");
	UnloadFont(renderData.font);

	for(int i=IMAGE_COUNT; i--;) UnloadImage(renderData.images[i]);
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

void DrawBoardCorner(Image* image, BoardGraphics graphics, BoardRenderData renderData, Rectangle rectangle, Field field)
{
	char commentBuffer[FIELD_NAME_LENGTH];
	switch(field.type)
	{
		case PROPERTY:
			ImageDrawRectangleRec(image, (Rectangle){
				rectangle.x,
				rectangle.y,
				rectangle.width,
				graphics.dividerOffset,
			}, field.color);
			ImageDrawRectangleRec(image, (Rectangle){
				rectangle.x + rectangle.width - graphics.dividerOffset,
				rectangle.y + graphics.dividerOffset,
				graphics.dividerOffset,
				rectangle.height - graphics.dividerOffset,
			}, field.color);
			uint64_t offset = graphics.dividerOffset + graphics.borderWidth;
			ImageDrawRectangleRec(image, (Rectangle){
				rectangle.x,
				rectangle.y + offset,
				rectangle.width - offset,
				rectangle.height - offset,
			}, renderData.colorPalette.light);
			snprintf(commentBuffer, FIELD_NAME_LENGTH, "$%i", field.value);
			break;
		case ACTION:
			ImageDrawRectangleRec(image, rectangle, renderData.colorPalette.light);

			Image icon = renderData.images[field.imageId];
			ImageDrawImagePro(image, icon,
				(Rectangle){0, 0, icon.width, icon.height},
				(Rectangle){
					rectangle.x + (rectangle.width - renderData.imageSizeEdge)/2,
					rectangle.y + (rectangle.height - renderData.imageSizeEdge)/2,
					renderData.imageSizeEdge,
					renderData.imageSizeEdge,
				},
				(Vector2){0}, 0,
				WHITE
			);
			
			strncpy(commentBuffer, field.comment, FIELD_NAME_LENGTH);
			break;
		default:
			break;
	}
	ImageDrawTextSpec(image,
		renderData.font,
		field.name,
		(Vector2){
			rectangle.x + renderData.nameOffsetCorner,
			rectangle.y + rectangle.height - renderData.nameOffsetCorner,
		},
		45,
		renderData.fontSize,
		renderData.fontSpacing,
		renderData.colorPalette.dark
		);
	ImageDrawTextSpec(image,
		renderData.font,
		commentBuffer,
		(Vector2){
			rectangle.x + renderData.commentOffsetCorner,
			rectangle.y + rectangle.height - renderData.commentOffsetCorner,
		},
		45,
		renderData.fontSize,
		renderData.fontSpacing,
		renderData.colorPalette.dark
		);
}

void DrawBoardEdge(Image* image, BoardGraphics graphics, BoardRenderData renderData, Rectangle rectangle, Field field)
{
	char commentBuffer[FIELD_NAME_LENGTH];
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
			snprintf(commentBuffer, FIELD_NAME_LENGTH, "$%i", field.value);
			break;
		case ACTION:
			ImageDrawRectangleRec(image, rectangle, renderData.colorPalette.light);

			Image icon = renderData.images[field.imageId];
			ImageDrawImagePro(image, icon,
				(Rectangle){0, 0, icon.width, icon.height},
				(Rectangle){
					rectangle.x + (rectangle.width - renderData.imageSizeEdge)/2,
					rectangle.y + (rectangle.height - renderData.imageSizeEdge)/2,
					renderData.imageSizeEdge,
					renderData.imageSizeEdge,
				},
				(Vector2){0}, 0,
				WHITE
			);
			
			strncpy(commentBuffer, field.comment, FIELD_NAME_LENGTH);
			break;
		default:
			break;
	}
	ImageDrawTextSpec(image,
		renderData.font,
		field.name,
		(Vector2){
			rectangle.x + rectangle.width/2,
			rectangle.y + rectangle.height - renderData.nameOffsetEdge,
		},
		0,
		renderData.fontSize,
		renderData.fontSpacing,
		renderData.colorPalette.dark
		);
	ImageDrawTextSpec(image,
		renderData.font,
		commentBuffer,
		(Vector2){
			rectangle.x + rectangle.width/2,
			rectangle.y + rectangle.height - renderData.commentOffsetEdge,
		},
		0,
		renderData.fontSize,
		renderData.fontSpacing,
		renderData.colorPalette.dark
		);
}

Vector2 CalculateFieldCenter(Board board, uint16_t fieldNumber)
{
	BoardGraphics graphics = board.graphics;
	uint16_t quarter = board.fieldCount/4;
	uint64_t cornerOffset = graphics.borderWidth + graphics.cornerSize/2;
	
	Vector2 position = {0};

	uint16_t sideId = fieldNumber/quarter;
	uint16_t sideFieldId = fieldNumber%quarter;
	if(sideFieldId == 0)
	{
		switch(sideId)
		{
			case 0:
				position = (Vector2){
					cornerOffset,
					graphics.pixelSize - cornerOffset,
				};
				break;
			case 1:
				position = (Vector2){
					cornerOffset,
					cornerOffset,
				};
				break;
			case 2:
				position = (Vector2){
					graphics.pixelSize - cornerOffset,
					cornerOffset,
				};
				break;
			case 3:
				position = (Vector2){
					graphics.pixelSize - cornerOffset,
					graphics.pixelSize - cornerOffset,
				};
				break;
		}
	}
	else
	{
		uint64_t edgeOffsetBase = cornerOffset*2 + graphics.fieldWidth/2;
		uint64_t edgeOffset = graphics.fieldWidth + graphics.borderWidth;

		uint64_t edgeOffsetFinal =  edgeOffsetBase + edgeOffset*(sideFieldId - 1);
		switch(sideId)
		{
			case 0:
				position = (Vector2){
					cornerOffset,
					graphics.pixelSize - edgeOffsetFinal,
				};
				break;
			case 1:
				position = (Vector2){
					edgeOffsetFinal,
					cornerOffset,
				};
				break;
			case 2:
				position = (Vector2){
					graphics.pixelSize - cornerOffset,
					edgeOffsetFinal,
				};
				break;
			case 3:
				position = (Vector2){
					graphics.pixelSize - edgeOffsetFinal,
					graphics.pixelSize - cornerOffset,
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
