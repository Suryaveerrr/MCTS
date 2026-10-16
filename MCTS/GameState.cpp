#include "GameState.h"


#include <iostream>
GameState::GameState()
{

}

/**
* setAndApplyAction will update the game state with the provided action and update the game board
*/
void GameState::SetAndApplyAction(GameAction newAction)
{
	gameAction = newAction;

	// update the board
	gameBoard.SetBoardSquare(newAction.row, newAction.col, newAction.playerMove);

}

/**
* setAndApplyAction will return all possible moves from the stored board state
*/
std::vector<std::pair<int, int>> GameState::GetPossibleMoves()
{
	std::vector<std::pair<int, int>> possibleMoves;

	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			if (gameBoard.GetBoardSquare(row,col) == BOARD_SQUARE_STATE::NONE)
			{
				// if currently empty,add to possible moves
				possibleMoves.push_back(std::make_pair(row, col));
			}
		}
	}

	return possibleMoves;
}

/**
* checkWin will check if the stored board state is a winning one for either marker and return the winnig marker
* If there is no winning marker, NONE will be returned
*/

BOARD_SQUARE_STATE GameState::CheckWin()
{

	// check each row for a match
	for (int i = 0; i < 3; i++)
	{
		if (gameBoard.GetBoardSquare(i,0) != BOARD_SQUARE_STATE::NONE &&
			gameBoard.GetBoardSquare(i,0) == gameBoard.GetBoardSquare(i,1) &&
			gameBoard.GetBoardSquare(i,0) == gameBoard.GetBoardSquare(i,2))
		{
			return gameBoard.GetBoardSquare(i,0);
		}

	}

	// Check each column
	for (int i = 0; i < 3; i++)
	{
		if (gameBoard.GetBoardSquare(0,i) != BOARD_SQUARE_STATE::NONE &&
			gameBoard.GetBoardSquare(0,i) == gameBoard.GetBoardSquare(1,i) &&
			gameBoard.GetBoardSquare(0,i) == gameBoard.GetBoardSquare(2,i))
		{
			return gameBoard.GetBoardSquare(0, i);
		}
	}

	// Check first diagonal
	if (gameBoard.GetBoardSquare(0,0) != BOARD_SQUARE_STATE::NONE &&
		gameBoard.GetBoardSquare(0,0) == gameBoard.GetBoardSquare(1,1) &&
		gameBoard.GetBoardSquare(0,0) == gameBoard.GetBoardSquare(2,2))
	{
		return gameBoard.GetBoardSquare(0,0);
	}

	// Check second diagonal
	if (gameBoard.GetBoardSquare(0,2) != BOARD_SQUARE_STATE::NONE &&
		gameBoard.GetBoardSquare(0,2) == gameBoard.GetBoardSquare(1,1) &&
		gameBoard.GetBoardSquare(0,2) == gameBoard.GetBoardSquare(2,0))
	{
		return gameBoard.GetBoardSquare(0,2);
	}

	// If no-one wins return the default state
	return BOARD_SQUARE_STATE::NONE;
}