#include <board_assets.h>
#include <board_rendering.h>

#include <raygui.h>

BoardAssets LoadBoardAssets()
{
	BoardAssets assets = {
		.colors = {
			.light = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL)),
			.dark = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL)),
		},
		.buildingLevelModelId = {
			SITE_MODEL,
			HOUSE_MODEL,
			VILLA_MODEL,
			APARTAMENT_MODEL,
		},
	};
	
	assets.font = LoadFontEx("resource/font/nihonium113.regular.ttf", 140, 0, 0);



	return assets;
}

void UnloadBoardAssets(BoardAssets assets)
{
	UnloadFont(assets.font);
}

void GenerateBoardTexture(Board* board)
{
	BoardLayout layout = board->layout;
	int quarter = board->fieldCount/4;

	float cornerOffset = layout.fieldHeight + layout.borderWidth * 2;
	float edgeOffset = layout.fieldWidth + layout.borderWidth;

	float sideOffset = cornerOffset + edgeOffset * (float)(quarter-1) - layout.borderWidth;
	float boardSize = sideOffset + cornerOffset;
	board->layout.boardSize = boardSize;
	board->layout.modelScale = layout.modelScaleMultiplier * layout.dividerOffset / boardSize;
	
	Image image = GenImageColor((int)boardSize, (int)boardSize, board->assets.colors.dark);
	ImageDrawRectangleRec(&image, (Rectangle){cornerOffset, cornerOffset, sideOffset-cornerOffset, sideOffset-cornerOffset}, board->assets.colors.light);

	//Down
	DrawBoardCorner(&image, *board, (Rectangle){
		layout.borderWidth,
		layout.borderWidth + sideOffset,
		layout.fieldHeight,
		layout.fieldHeight,
		}, 0);
	//Left
	DrawBoardCorner(&image, *board, (Rectangle){
		layout.borderWidth,
		layout.borderWidth,
		layout.fieldHeight,
		layout.fieldHeight,
		}, quarter);
	//Up
	DrawBoardCorner(&image, *board, (Rectangle){
		layout.borderWidth + sideOffset,
		layout.borderWidth,
		layout.fieldHeight,
		layout.fieldHeight,
		}, quarter * 2);
	//Right
	DrawBoardCorner(&image, *board, (Rectangle){
		layout.borderWidth + sideOffset,
		layout.borderWidth + sideOffset,
		layout.fieldHeight,
		layout.fieldHeight,
		}, quarter * 3);

	for(int i=1; i<quarter; ++i)
	{
		DrawBoardEdge(&image, *board, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth,
			layout.fieldWidth,
			layout.fieldHeight,
			}, quarter + i);
		
		DrawBoardEdge(&image, *board, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth + sideOffset,
			layout.fieldWidth,
			layout.fieldHeight,
			}, 4*quarter - i);
	}
	ImageRotateCCW(&image);
	for(int i=1; i<quarter; ++i)
	{
		DrawBoardEdge(&image, *board, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth,
			layout.fieldWidth,
			layout.fieldHeight,
			}, 2*quarter + i);
		
		DrawBoardEdge(&image, *board, (Rectangle){
			cornerOffset + (float)(i-1) * edgeOffset,
			layout.borderWidth + sideOffset,
			layout.fieldWidth,
			layout.fieldHeight,
			}, quarter - i);
	}
	ImageRotateCW(&image);

	UnloadTexture(textures[BOARD_TEXTURE]);
	textures[BOARD_TEXTURE] = LoadTextureFromImage(image);
	models[BOARD_MODEL].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textures[BOARD_TEXTURE];
	UnloadImage(image);
}
