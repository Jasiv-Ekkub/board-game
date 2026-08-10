#include <board_assets.h>
#include <board_rendering.h>

#include <raygui.h>

BoardAssets LoadBoardAssets()
{
	BoardAssets assets = {
		.shader = LoadShader(
		"resource/shaders/shader.vs",
		"resource/shaders/shader.fs"
		),
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

	assets.images[START_IMAGE] = LoadImageDefault();
	assets.images[PRISON_IMAGE] = LoadImageDefault();
	assets.images[PARKING_IMAGE] = LoadImageDefault();
	assets.images[POLICEMAN_IMAGE] = LoadImageDefault();

	assets.textures[BOARD_TEXTURE] = LoadTextureDefault();
	assets.models[BOARD_MODEL] = LoadModelFromMesh(GenMeshPlane(2, 2, 4, 4));
	assets.models[BOARD_MODEL].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = assets.textures[BOARD_TEXTURE];

	assets.models[PAWN_MODEL] = LoadModel("resource/models/pawn.glb");
	assets.models[SITE_MODEL] = LoadModel("resource/models/placeholder_box.glb");
	assets.models[HOUSE_MODEL] = LoadModel("resource/models/placeholder_cone.glb");
	assets.models[VILLA_MODEL] = LoadModel("resource/models/placeholder_sphere.glb");
	assets.models[APARTAMENT_MODEL] = LoadModel("resource/models/placeholder_cylinder.glb");
	
	for(int i=0; i<BOARD_MODEL_AMOUNT; ++i)
	{
		if(IsModelValid(assets.models[i]))
		{
			assets.models[i].materials[0].shader = assets.shader;
		}
	}
	return assets;
}

void UnloadBoardAssets(BoardAssets assets)
{
	UnloadFont(assets.font);
	UnloadShader(assets.shader);
	for(int i=0; i<BOARD_MODEL_AMOUNT; ++i)
	{
		UnloadModel(assets.models[i]);
	}
	for(int i=0; i<BOARD_TEXTURE_AMOUNT; ++i)
	{
		UnloadTexture(assets.textures[i]);
	}
	for(int i=0; i<BOARD_IMAGE_AMOUNT; ++i)
	{
		UnloadImage(assets.images[i]);
	}
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

	UnloadTexture(board->assets.textures[BOARD_TEXTURE]);
	board->assets.textures[BOARD_TEXTURE] = LoadTextureFromImage(image);
	board->assets.models[BOARD_MODEL].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = board->assets.textures[BOARD_TEXTURE];
	UnloadImage(image);
}
