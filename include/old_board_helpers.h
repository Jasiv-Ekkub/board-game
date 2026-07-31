#ifndef BOARD_HELPERS_H
#define BOARD_HELPERS_H

/*
#include <board.h>
#include <image_defines.h>

typedef struct BoardRenderData
{
	
	struct
	{
		Color light;
		Color dark;
	} colorPalette;

	Image images[IMAGE_COUNT];
} BoardRenderData;

void LoadFieldsDefault(Board* board);
void LoadModels(Board* board);
void UnloadModels(Board board);

BoardGraphics LoadGraphics();

BoardRenderData LoadRenderData();
void UnloadRenderData(BoardRenderData renderData);

void DrawBoardCorner(Image* image, BoardGraphics graphics, BoardRenderData renderData, Rectangle rectangle, Field field);
void DrawBoardEdge(Image* image, BoardGraphics graphics, BoardRenderData renderData, Rectangle rectangle, Field field);

void ImageDrawTextSpec(Image* image, Font font, const char* text, Vector2 position, int rotation, float fontSize, float spacing, Color tint);

Vector3 CalculatePlayerPosition(Board board, int playerId);
Vector3 CalculateHousePosition(Board board, int fieldId);
*/

#endif //BOARD_HELPERS_H
