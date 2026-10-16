#pragma once

#include "GameBoard.h"
#include <vector>


/*
* GameState class represent current state of a tic tac toe game
*/
class GameState
{
public:
	GameBoard gameBoard;
	GameAction gameAction;

	GameState();

	// state changing functions
	void SetAndApplyAction(GameAction newAction);

	// status checking
	std::vector<std::pair<int, int>> GetPossibleMoves();
	BOARD_SQUARE_STATE CheckWin();

};

