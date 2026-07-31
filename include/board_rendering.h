#ifndef BOARD_RENDERING_H
#define BOARD_RENDERING_H

#include <board.h>
#include <raylib.h>

Image LoadImageDefault();
Texture2D LoadTextureDefault();
void DrawBoardCorner(Image* image, Board board, Rectangle rectangle, int id);

#endif //BOARD_RENDERING_H
