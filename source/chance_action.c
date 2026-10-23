#include <chance_action.h>
#include <stdlib.h>
#include <stdio.h>

#define CARDS_COUNT 5

void FoundMoney(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

    int amount = (rand()%19) * 50 + 100;
	player->money += amount;

    char buffer[MAX_POPUP_LENGTH];
    snprintf(buffer, MAX_POPUP_LENGTH, "Player found $%i on a street", amount);
	AddPopupBoard(board, buffer);
	PlaySound(sounds[KA_CHING_SOUND]);
}

void LostMoney(Board* board)
{
	Player* player = &board->players[board->currentPlayer];

    int amount = (rand()%19) * 50 + 100;
	player->money -= amount;

    char buffer[MAX_POPUP_LENGTH];
    snprintf(buffer, MAX_POPUP_LENGTH, "Player lost $%i on a street", amount);
	AddPopupBoard(board, buffer);
	PlaySound(sounds[KA_CHING_SOUND]);
}

void LostLicence(Board* board)
{
	Player* player = &board->players[board->currentPlayer];
	
    int amount = rand()%3 + 1;
    player->turnSkips = amount;

    char buffer[MAX_POPUP_LENGTH];
    snprintf(buffer, MAX_POPUP_LENGTH, "Player lost their driving licence\nThey will be unable to move for %i rounds", amount);
	AddPopupBoard(board, buffer);
}

void RollDiceAgain(Board* board)
{
	board->phase = START_ROUND;
	AddPopupBoard(board, "Player roll dice again");
}

void MoveForward(Board* board)
{
    board->phase = MOVE_PLAYER;
    
    int amount = rand()%3 + 1;
    board->currentDiceroll = amount;

    char buffer[MAX_POPUP_LENGTH];
    snprintf(buffer, MAX_POPUP_LENGTH, "Player moves %i fields forward", amount);
	AddPopupBoard(board, buffer);
}

FieldAction cards[CARDS_COUNT] = {
    MoveForward,
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
