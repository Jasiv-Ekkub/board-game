#include <chance_action.h>
#include <stdlib.h>

#define CARDS_COUNT 4

void FoundMoney(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

	player->money += 500;
	AddPopupBoard(board, "Player found $500 on a street");
	PlaySound(sounds[KA_CHING_SOUND]);
}

void LostMoney(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

	player->money -= 500;
	AddPopupBoard(board, "Player lost $500 somewhere");
	PlaySound(sounds[KA_CHING_SOUND]);
}

void LostLicence(Board* board)
{
	Player* player = &board->players[board->currentPlayer];
	player->turnSkips = 3;
	AddPopupBoard(board, "Player lost their driving licence\nThey will be unable to move for 3 rounds");
}

void RollDiceAgain(Board* board)
{
	board->phase = START_ROUND;
	AddPopupBoard(board, "Player roll dice again");
}

FieldAction cards[CARDS_COUNT] = {
	FoundMoney,
	LostMoney,
	LostLicence,
	RollDiceAgain
};

void ChanceAction(Board* board)
{
	cards[rand()%CARDS_COUNT](board);
	AddPopupBoard(board, "Chance - draw a card");
}
