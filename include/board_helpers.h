#ifndef BOARD_HELPERS_H
#define BOARD_HELPERS_H

#include <board.h>
#include <image_defines.h>

typedef struct BoardRenderData
{
	Font font;
	float fontSize;
	float fontSpacing;

	uint64_t nameOffsetEdge;
	uint64_t commentOffsetEdge;
	uint64_t imageSizeEdge;

	uint64_t nameOffsetCorner;
	uint64_t commentOffsetCorner;
	uint64_t imageSizeCorner;
	
	struct
	{
		Color light;
		Color dark;
	} colorPalette;

	Image images[IMAGE_COUNT];
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
