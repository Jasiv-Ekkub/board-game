#ifndef BOARD_SETUP_H
#define BOARD_SETUP_H

#include <board.h>

void LoadFields(Board* board);
void LoadPlayers(Board* board, int startMoney, int humanCount, int botCount);
void GenerateBoardTexture(Board* board);

#endif //BOARD_SETUP_H
