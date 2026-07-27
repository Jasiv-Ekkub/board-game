#ifndef BOARD_HELPERS_H
#define BOARD_HELPERS_H

#include <board.h>

#define START_IMAGE 0
#define PRISON_IMAGE 1
#define PARKING_IMAGE 2
#define POLICEMAN_IMAGE 3

typedef struct BoardRenderData
{
	Font font;
	float fontSize;
	float fontSpacing;

	int nameOffset;
	int commentOffset;

	struct
	{
		Color light;
		Color dark;
	} colorPalette;

	int cornerImageScale;

	Image cornerImages[4];
} BoardRenderData;

void LoadFieldsDefault(Board* board);

BoardGraphics LoadGraphics();

BoardRenderData LoadRenderData();
void UnloadRenderData(BoardRenderData renderData);

void DrawBoardCorner(Image* image, BoardGraphics graphics, BoardRenderData renderData, Rectangle rectangle, Field field);
void DrawBoardEdge(Image* image, BoardGraphics graphics, BoardRenderData renderData, Rectangle rectangle, Field field);

void ImageDrawTextSpec(Image* image, Font font, const char* text, Vector2 position, float rotation, float fontSize, float spacing, Color tint);

Vector2 CalculateFieldCenter(Board board, uint16_t fieldNumber);

#endif //BOARD_HELPERS_H
