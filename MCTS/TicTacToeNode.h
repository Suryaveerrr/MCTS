#pragma once

#include <vector>

#include "GameBoard.h"
#include "GameState.h"

/*
* TicTacToeNode represents a node on a Tic Tac Tow MCTS tree
*/
class TicTacToeNode
{

public:
	TicTacToeNode();
	~TicTacToeNode();


	//gets and sets
	void setGameState(GameState newState);
	GameState getGameState() { return worldState; }

	bool getIsEndState() { return isEndState; }
	std::vector<TicTacToeNode*> getBranches() { return branches; }
	int getWins() { return wins; }
	int getVisits() { return visits; }
	BOARD_SQUARE_STATE getActivePlayer() { return activePlayer; }
	void setParent(TicTacToeNode* newParent) { parent = newParent; }
	void setActivePlayer(BOARD_SQUARE_STATE active) { activePlayer = active; }
	

	// MCTS steps
	TicTacToeNode* Select();
	TicTacToeNode* Expand();
	void Simulate(BOARD_SQUARE_STATE playerTurn);
	void Backpropagate(int reward);

	// tree functions
	TicTacToeNode* FindHighestRankingChild();
	void resetNode();
	void CalcResult();

	//status
	std::vector<std::pair<int, int>> availableMoves; // unexplored branches

private:

	//MCTS Node data
	int wins; // number of times won
	int visits; // number of times visited

	//tree variables
	std::vector<TicTacToeNode*> branches; // child nodes
	TicTacToeNode* parent; // parent node

	// status tracking
	GameState worldState; // game state at this node
	bool isEndState; // if eitehr game is won or no moves possible
	BOARD_SQUARE_STATE activePlayer; // the player who moved to create this state - child nodes should use opposite marker

	void generatePossibleMoves();
	BOARD_SQUARE_STATE simulationWinner;
};

