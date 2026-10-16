#include "MCTS_Reasoner.h"
#include <iostream>


bool MCTS_Reasoner::Init(std::string _id, AIBrainBlackboardBase& _context)
{
	actorBlackboard = &_context;
	reasonerID = _id;
	Global_Blackboard = AIBlackboard_GlobalTTT::getInstance();
	return true;

}

void MCTS_Reasoner::Think()
{

	// create a new MCTS TicTacToeNode which will be used to create the tree
	rootNode = new TicTacToeNode();

	//update blackboard for status tracking
	GameState _mainState = Global_Blackboard->GetValue<GameState>("GameState");
	BOARD_SQUARE_STATE _playerMarker = (BOARD_SQUARE_STATE)Global_Blackboard->GetValue<int>("PlayerMarker");
	BOARD_SQUARE_STATE _aiMarker = (BOARD_SQUARE_STATE)Global_Blackboard->GetValue<int>("AIMarker");
	int _maxRuns = (BOARD_SQUARE_STATE)Global_Blackboard->GetValue<int>("MaxRuns");

	//set the active player and the current main game state
	rootNode->setActivePlayer(_playerMarker); 
	rootNode->setGameState(_mainState);


	// var to track the number of iterations
	int runCount = 0;


	// Apply the MCTS algorithm to determine the AI's move
	// - will run for maximum of MAX_RUNS iterations
	do {

		// Select - choose a node that will be expanded
		TicTacToeNode* selectedNode = rootNode->Select();

		// Expand - expand the tree from this node
		TicTacToeNode* expandedNode = selectedNode->Expand();

		// expandedNode will be NULL if it's a leaf node and simulation is not possible
		if (!expandedNode == NULL)
		{
			// Simulate - Simulate to an end point, starting with aiMarker turn. 	
			expandedNode->Simulate(_aiMarker);

			// CalcResult will trigger the back propagation of the result to the root node
			expandedNode->CalcResult();
		}

		runCount++;

	} while (runCount < _maxRuns);



	// perform the action - find child node with highest ranking
	TicTacToeNode* highestChild = rootNode->FindHighestRankingChild();
	GameAction bestAction = highestChild->getGameState().gameAction;

	// only a single option - make a move
	selectedOption = options[0].get();

	// update the blackboard with the selected action
	actorBlackboard->EditValue<int>("Row", bestAction.row);
	actorBlackboard->EditValue<int>("Col", bestAction.col);

	//track data for output
	std::vector < std::pair<int,int> >  _availableMoves = _mainState.GetPossibleMoves();

	std::vector <int> _nodeVisits;
	std::vector <int> _nodeWins;
	std::vector <int> _nodeActionRow;
	std::vector <int> _nodeActionCol;

	Global_Blackboard->EditValue<AIMath_Global::Vector2f>("AIMove", AIMath_Global::Vector2f(bestAction.col, bestAction.row));

	for (int i = 0; i < rootNode->getBranches().size(); i++)
	{
		int _r = rootNode->getBranches()[i]->getGameState().gameAction.row;
		int _c = rootNode->getBranches()[i]->getGameState().gameAction.col;

		_nodeWins.push_back(rootNode->getBranches()[i]->getWins());
		_nodeVisits.push_back(rootNode->getBranches()[i]->getVisits());

		_nodeActionCol.push_back(_c);
		_nodeActionRow.push_back(_r);


		
	}

	//update data for output
	Global_Blackboard->EditValue<std::vector<int>>("AINodeWins", _nodeWins);
	Global_Blackboard->EditValue<std::vector<int>>("AINodeVisits", _nodeVisits);
	Global_Blackboard->EditValue<std::vector<int>>("AINodeRow", _nodeActionRow);
	Global_Blackboard->EditValue<std::vector<int>>("AINodeCol", _nodeActionCol);

	
	delete rootNode;




}






