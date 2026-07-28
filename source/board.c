#include <board.h>
#include <board_helpers.h>
#include <string.h>
#include <stdio.h>

Texture2D LoadBoardTexture(Board* board, BoardRenderData renderData);

void DebugPrintAllFields(Board board)
{
	for(uint16_t i=0; i<board.fieldCount; ++i)
	{
		Field* field = board.fields + i;
		printf("name:%s, value:%i, buildingLevel:%i, ownerId:%i, color:%i\n", field->name, field->value, field->buildingLevel, field->ownerId, field->color);
	}
}

#warning LoadBoard: filename does nothing
Board LoadBoard(const char* filename, uint16_t size)
{
	if(size > 256) size = 256;
	else if(size < 16) size = 16;
	else size &= 0xFFFC;

	Board board = {
		.fieldCount = size,
		.playerCount = 4,
		.players = {
			{"Player A", 0, RED},
			{"Player B", 0, GREEN},
			{"Player C", 0, BLUE},
			{"Player D", 0, YELLOW},
		},
		.FOOBAR = 0,
	};

	LoadFieldsDefault(&board);
	board.graphics = LoadGraphics();

	BoardRenderData renderData = LoadRenderData();
	board.boardTexture = LoadBoardTexture(&board, renderData);
	UnloadRenderData(renderData);

	board.boardModel = LoadModelFromMesh(GenMeshPlane(2, 2, 3, 4));
	board.boardModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = board.boardTexture;

	board.pawnShader = LoadShader("resource/shaders/directional_light.vs", "resource/shaders/directional_light.fs");
	board.pawnModel = LoadModel("resource/models/pawn.glb");
	board.pawnModel.materials[0].shader = board.pawnShader;

	printf("GAMEINFO: Loaded board\n");
	return board;
}

void UpdateBoard(Board* board, GameContext gameContext)
{
	Vector3 position = {0};
	//DrawTexturePro(board->boardTexture, (Rectangle){0, 0, board->boardTexture.width, board->boardTexture.height}, (Rectangle){5, 5, 800, 800}, (Vector2){0}, 0, WHITE);
	DrawModel(board->boardModel, position, 1, WHITE);

	board->FOOBAR += gameContext.deltaTime;
	uint8_t cond = 0;
	if(board->FOOBAR > 0.5)
	{
		cond = 1;
		board->FOOBAR = 0;
	}

	for(int i=0; i<board->playerCount; ++i)
	{
		if(cond)
		{
			board->players[i].position += 1+i;
			board->players[i].position %= board->fieldCount;
			cond = 0;
		}
		Player player = board->players[i];
		Vector2 offset = CalculateFieldCenter(*board, player.position);
		
		if(i%2) offset.x -= 0.03f;
		else offset.x += 0.03f;
		if(i/2) offset.y -= 0.03f;
		else offset.y += 0.03f;

		Vector3 pawnPosition = {position.x + offset.x, position.y, position.z + offset.y};
		DrawModel(board->pawnModel, pawnPosition, 0.025, player.color);
	}
}

void UnloadBoard(Board board)
{
	UnloadTexture(board.boardTexture);
	UnloadModel(board.boardModel);

	UnloadModel(board.pawnModel);
	UnloadShader(board.pawnShader);
	printf("GAMEINFO: Unloaded board\n");
}

Texture2D LoadBoardTexture(Board* board, BoardRenderData renderData)
{
	uint16_t quarter = board->fieldCount/4;
	const BoardGraphics graphics = board->graphics;

	uint64_t cornerOffset = graphics.cornerSize + graphics.borderWidth * 2;
	uint64_t edgeOffset = graphics.fieldWidth + graphics.borderWidth;

	uint64_t sideOffset = cornerOffset + edgeOffset * (quarter-1) - graphics.borderWidth;

	uint64_t pixelSize = sideOffset + cornerOffset;
	board->graphics.pixelSize = pixelSize;
	
	Image image = GenImageColor(pixelSize, pixelSize, renderData.colorPalette.dark);

	//Down
	DrawBoardCorner(&image, graphics, renderData, (Rectangle){
		graphics.borderWidth,
		graphics.borderWidth + sideOffset,
		graphics.cornerSize,
		graphics.cornerSize,
		}, board->fields[0]);
	//Left
	DrawBoardCorner(&image, graphics, renderData, (Rectangle){
		graphics.borderWidth,
		graphics.borderWidth,
		graphics.cornerSize,
		graphics.cornerSize,
		}, board->fields[quarter]);
	//Up
	DrawBoardCorner(&image, graphics, renderData, (Rectangle){
		graphics.borderWidth + sideOffset,
		graphics.borderWidth,
		graphics.cornerSize,
		graphics.cornerSize,
		}, board->fields[quarter * 2]);
	//Right
	DrawBoardCorner(&image, graphics, renderData, (Rectangle){
		graphics.borderWidth + sideOffset,
		graphics.borderWidth + sideOffset,
		graphics.cornerSize,
		graphics.cornerSize,
		}, board->fields[quarter * 3]);

	for(uint16_t i=1; i<quarter; ++i)
	{
		DrawBoardEdge(&image, graphics, renderData, (Rectangle){
			cornerOffset + (i-1) * edgeOffset,
			graphics.borderWidth,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board->fields[quarter + i]);
		
		DrawBoardEdge(&image, graphics, renderData, (Rectangle){
			cornerOffset + (i-1) * edgeOffset,
			graphics.borderWidth + sideOffset,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board->fields[4*quarter - i]);
	}
	ImageRotateCCW(&image);
	for(uint16_t i=1; i<quarter; ++i)
	{
		DrawBoardEdge(&image, graphics, renderData, (Rectangle){
			cornerOffset + (i-1) * edgeOffset,
			graphics.borderWidth,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board->fields[2*quarter + i]);
		
		DrawBoardEdge(&image, graphics, renderData, (Rectangle){
			cornerOffset + (i-1) * edgeOffset,
			graphics.borderWidth + sideOffset,
			graphics.fieldWidth,
			graphics.cornerSize,
			}, board->fields[quarter - i]);
	}
	ImageRotateCW(&image);

	ImageDrawRectangleRec(&image, (Rectangle){cornerOffset, cornerOffset, sideOffset-cornerOffset, sideOffset-cornerOffset}, renderData.colorPalette.light);
	ImageDrawTextSpec(&image, renderData.font, "Board game", (Vector2){pixelSize/2, pixelSize/2}, 45, 120, 1, renderData.colorPalette.dark);

	Texture2D texture = LoadTextureFromImage(image);
	UnloadImage(image);
	GenTextureMipmaps(&texture);
	return texture;
}
