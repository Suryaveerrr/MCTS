#pragma once
#include "../AI_Framework/framework.h"
#include "TicTacToeNode.h"
#include "AIBlackboard_GlobalTTT.h"

class MCTS_Reasoner :
    public AIReasonerBase
{
public:
	bool Init(std::string _id, AIBrainBlackboardBase& _context) override;
	void Think() override;

private:
	TicTacToeNode* rootNode;

	AIBlackboard_GlobalTTT *Global_Blackboard;
};

