#ifndef BOARD_ASSETS_H
#define BOARD_ASSETS_H

#include <raylib.h>
#include <assets.h>

typedef struct Board Board;
typedef struct BoardAssets
{
	Font font;
	
	struct {
		Color light;
		Color dark;
	} colors;

	ModelId buildingLevelModelId[4];
} BoardAssets;


BoardAssets LoadBoardAssets();
void UnloadBoardAssets(BoardAssets assets);

void GenerateBoardTexture(Board* board);

#endif //BOARD_ASSETS_H
