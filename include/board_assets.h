#ifndef BOARD_ASSETS_H
#define BOARD_ASSETS_H

#include <raylib.h>

typedef struct Board Board;

typedef enum BoardModelId
{
	BOARD_MODEL=0,
	PAWN_MODEL,
	SITE_MODEL,
	HOUSE_MODEL,
	VILLA_MODEL,
	APARTAMENT_MODEL,
	//===
	BOARD_MODEL_AMOUNT,
} BoardModelId;


typedef enum BoardTextureId
{
	BOARD_TEXTURE=0,
	//===
	BOARD_TEXTURE_AMOUNT,

} BoardTextureId;


typedef enum BoardImageId
{
	START_IMAGE=0,
	PRISON_IMAGE,
	PARKING_IMAGE,
	POLICEMAN_IMAGE,
	//===
	BOARD_IMAGE_AMOUNT,

} BoardImageId;


typedef struct BoardAssets
{
	Shader shader;
	Font font;
	
	struct {
		Color light;
		Color dark;
	} colors;

	Model models[BOARD_MODEL_AMOUNT];
	Texture textures[BOARD_TEXTURE_AMOUNT];
	Image images[BOARD_IMAGE_AMOUNT];
	BoardModelId buildingLevelModelId[4];
} BoardAssets;


BoardAssets LoadBoardAssets();
void UnloadBoardAssets(BoardAssets assets);

void GenerateBoardTexture(Board* board);

#endif //BOARD_ASSETS_H
