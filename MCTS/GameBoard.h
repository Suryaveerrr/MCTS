#pragma once

/*
GameBoard defines enums and structures for the Tic Tac Toe game
 - BOARD_SQUARE_STATE used for markers
 - GameBoard to represent a 3x3 game board
 - GameAction to represent an action taken by a player (i.e. place marker X in row 1, col 2)

*/
#include <vector>

// define enum's and structs used in the game

// BOARD_SQUARE_STATE used for markers
enum BOARD_SQUARE_STATE { NONE, CIRCLE, CROSS };

// GameBoard to represent a 3x3 game board
struct GameBoard
{
	int boardWidth = 3;
	int boardHeight = 3;
	std::vector< BOARD_SQUARE_STATE> board;

public:
	GameBoard()
	{
		for (int i = 0; i < boardWidth; i++)
		{
			for (int i = 0; i < boardHeight; i++)
			{
				board.push_back(BOARD_SQUARE_STATE::NONE);
			}
		}
	}

	BOARD_SQUARE_STATE GetBoardSquare(int row, int col)
	{
		int _index = (row * boardWidth) + col;

		return board.at(_index);

	}

	void SetBoardSquare(int row, int col, BOARD_SQUARE_STATE newState)
	{
		int _index = (row * boardWidth) + col;

		board.at(_index) = newState;

	}
};


//GameAction to represent an action taken by a player (i.e. place marker X in row 1, col 2)
struct GameAction
{
	int row, col;
	BOARD_SQUARE_STATE playerMove;

public:
	GameAction(int newRow, int newCol, BOARD_SQUARE_STATE move)
	{
		row = newRow;
		col = newCol;
		playerMove = move;
	}

	GameAction()
	{
		playerMove = BOARD_SQUARE_STATE::NONE;
		row = 0;
		col = 0;
	}


};

