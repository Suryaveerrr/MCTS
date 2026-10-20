#include "TicTacToeNode.h"
#include <iostream>
#include "GameBoard.h"
#include "AIBlackboard_GlobalTTT.h"
#include "AIRandom_Global.h"


// general functions

BOARD_SQUARE_STATE getOppositeMove(BOARD_SQUARE_STATE state)
{
	// Flip to the other player
	if (state == BOARD_SQUARE_STATE::CIRCLE)
	{
		return BOARD_SQUARE_STATE::CROSS;
	}
	else if (state == BOARD_SQUARE_STATE::CROSS)
	{
		return BOARD_SQUARE_STATE::CIRCLE;
	}
	else
		return BOARD_SQUARE_STATE::NONE;
}

// class functions
TicTacToeNode::TicTacToeNode()
{
	isEndState = false;
	parent = nullptr;
	wins = 0;
	visits = 0;
	activePlayer = BOARD_SQUARE_STATE::NONE;
	simulationWinner = BOARD_SQUARE_STATE::NONE;

}

TicTacToeNode::~TicTacToeNode()
{
	// parent will be deleted by its parent so we don't need to clean it up here
	parent = nullptr;

	for (auto branch : branches)
	{
		delete branch;
	}
	branches.clear();
	availableMoves.clear();

}


/*
* Select - will perform the Select step of the MCTS algorithm
*  will select down a path according to the selection policy until leaf is found
*/
TicTacToeNode* TicTacToeNode::Select()
{

	if (availableMoves.size() > 0)
	{
		// still unexplored branches, explore
		return this;
	}

	if (isEndState )
	{
		// leaf node, return and will be handled by Reasoner
		return this;
	}


	// TASK TODO - replace below below code with UCT based selection
	//  - remember to call 'Select()' on the node 

	int randomBranch = rand() % branches.size();
	return branches[randomBranch]->Select();

	


}

/*
* Expand  - will perform the Expansion step of the MCTS algorithm
* will add a new node by randomly selecting from all possible moves
*/
TicTacToeNode* TicTacToeNode::Expand()
{

	if (isEndState)
	{
		//std::cout << "Cannont expand - leaf " << std::endl;
		return NULL; // if this is game end state, we cannot expand
	}
	else
	{
		// pick a random move from, the unexplored ones
		int randomMove = rand() % availableMoves.size();
		std::pair<int, int> move = availableMoves[randomMove];

		// delete the selected move from vector
		availableMoves.erase(availableMoves.begin() + randomMove);

		// create a new child node 
		TicTacToeNode* childNode = new TicTacToeNode();
		childNode->setParent(this);

		// flip the marker to other player
		BOARD_SQUARE_STATE nextTurn = getOppositeMove(activePlayer);
		childNode->setActivePlayer(nextTurn);

		// create the new world state
		GameState newWorldState;
		newWorldState.gameBoard = this->worldState.gameBoard;
		GameAction newAction(move.first, move.second, nextTurn);

		// apply the move to the board and set the child's state
		newWorldState.SetAndApplyAction(newAction);
		childNode->setGameState(newWorldState);

		// add the newly created node as a child of current node		
		branches.push_back(childNode);

		// reutrn node, for simulation
		return childNode;
	}

}
#/*
* Simulate  - will perform the Simulate step of the MCTS algorithm
* will play random moves until a end state is found, and set the simulationWinner
*/
void TicTacToeNode::Simulate(BOARD_SQUARE_STATE startingTurn)
{


	GameState simState = getGameState();

	// set the active player
	BOARD_SQUARE_STATE currentPlayer = getOppositeMove(startingTurn);

	// if the state already has a winner, record it and stop
	BOARD_SQUARE_STATE winner = simState.CheckWin();
	if (winner != BOARD_SQUARE_STATE::NONE)
	{
		simulationWinner = winner;
		return;
	}

	while (true)
	{
		// get all possible moves
		std::vector<std::pair<int, int>> moves = simState.GetPossibleMoves();

		// no moves left and no winner, so it is a draw
		if (moves.size() == 0)
		{
			simulationWinner = BOARD_SQUARE_STATE::NONE;
			return;
		}

		// randomly select one possible move
		int randomIndex = rand() % moves.size();
		std::pair<int, int> move = moves[randomIndex];

		// apply the move to the game state
		GameAction action(move.first, move.second, currentPlayer);
		simState.SetAndApplyAction(action);

		// check if that move won the game
		winner = simState.CheckWin();
		if (winner != BOARD_SQUARE_STATE::NONE)
		{
			simulationWinner = winner;
			return;
		}

		// otherwise the other player moves next
		currentPlayer = getOppositeMove(currentPlayer);
	}





	


}



/*
* CalcResult will determing if the AI won or lost, and trigger the back propagation
*/

void TicTacToeNode::CalcResult()
{

	BOARD_SQUARE_STATE _aiMarker = (BOARD_SQUARE_STATE)AIBlackboard_GlobalTTT::getInstance()->GetValue<int>("AIMarker");

	if (simulationWinner == _aiMarker)
	{
		Backpropagate(1);
	}
	else if (simulationWinner == BOARD_SQUARE_STATE::NONE)
	{
		Backpropagate(0);
	}
	else
	{
		Backpropagate(-1);
	}
}



/*
* Backpropagate  - will perform the Backpropagate step of the MCTS algorithm
* the result will be returned back up the tree branches to the root node
*/

void TicTacToeNode::Backpropagate(int result)
{

	// increment visits (every call)
	visits++;

	// increment wins only when the AI won the simulation (result 1)
	if (result == 1)
	{
		wins++;
	}

	// pass the result up the tree; the root has no parent, so parent is nullptr
	if (parent != nullptr)
	{
		parent->Backpropagate(result);
	}



}




// status tracking and updates



/**
*  setGameState will set the game state for the node, as well as calling generatePossibleMoves to update status
*/

void TicTacToeNode::setGameState(GameState newWorldState)
{
	worldState = newWorldState;
	generatePossibleMoves();
}


/**
*  generatePossibleMoves will calculate the possible mvoes from this node and set the isEndState variable appropriately
*  should only be called once when node's game state is defined
*/
void TicTacToeNode::generatePossibleMoves()
{

	availableMoves = worldState.GetPossibleMoves();

	if (availableMoves.size() == 0)
		isEndState = true;

	if (worldState.CheckWin() != BOARD_SQUARE_STATE::NONE)
		isEndState = true;
}





/*
* FindHighestRankingChild will determine which of this node's children has the highest metric
*/

TicTacToeNode* TicTacToeNode::FindHighestRankingChild()
{

	if (branches.size() == 0)
		return NULL;

	float maxWins = 0;
	int maxIndex = 0;

	for (int i = 0; i < branches.size(); i++)
	{

		float nodeWinRate = ((float)branches[i]->getWins()) / branches[i]->getVisits();

		if (nodeWinRate > maxWins)
		{
			maxIndex = i;
			maxWins = nodeWinRate;
		}
	}

	return branches[maxIndex];
}

#/*
* resetNode will return the node to its default values
*/

void TicTacToeNode::resetNode()
{
	branches.clear();
	wins = 0;
	visits = 0;
	availableMoves.clear();
}