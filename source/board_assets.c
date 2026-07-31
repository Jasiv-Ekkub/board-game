#include <board_assets.h>
#include <board_rendering.h>

BoardAssets LoadBoardAssets()
{
	BoardAssets assets = {
		.shader = LoadShader(
		"resource/shaders/shader.vs",
		"resource/shaders/shader.fs"
		),
		.colors = {
			.light = WHITE,
			.dark = BLACK,
		},
	};
	
	assets.images[START_IMAGE] = LoadImageDefault();
	assets.images[PRISON_IMAGE] = LoadImageDefault();
	assets.images[PARKING_IMAGE] = LoadImageDefault();
	assets.images[POLICEMAN_IMAGE] = LoadImageDefault();

	assets.textures[BOARD_TEXTURE] = LoadTextureDefault();
	assets.models[BOARD_MODEL] = LoadModelFromMesh(GenMeshPlane(2, 2, 4, 4));
	assets.models[BOARD_MODEL].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = assets.textures[BOARD_TEXTURE];

	assets.models[PAWN_MODEL] = LoadModel("resource/models/pawn.glb");
	assets.models[HOUSE_MODEL] = LoadModel("resource/models/house.glb");
	
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


	UnloadTexture(board->assets.textures[BOARD_TEXTURE]);
	board->assets.textures[BOARD_TEXTURE] = LoadTextureFromImage(image);
	board->assets.models[BOARD_MODEL].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = board->assets.textures[BOARD_TEXTURE];
	UnloadImage(image);
}
