#include "AIConstructor_MCTS.h"
#include "AIActor_TicTacToe.h"

bool AIConstructor_MCTS::Init() {

	method = AIConstructorBase::Method::MCTS;
	return true;
}



/*
DefineActions creates all the possible actions that the AI can perform
*/
void AIConstructor_MCTS::DefineActions()
{



	//Define the function for making a move
	auto makeMoveFunction = [](AIBrainBlackboardBase& bb) -> ActionStatus {
		AIActor_TicTacToe* actor = static_cast<AIActor_TicTacToe*>(bb.GetActorContext());

		int _row = actor->GetBBValue<int>("Row");
		int _col = actor->GetBBValue<int>("Col");

		actor->MakeMove(_row, _col);

		return ActionStatus::ACTION_SUCCESS;
		};


	// Link the function to an identifier
	AddActionByName("ActionMakeMove", makeMoveFunction);


}

/*
   DefineConsiderations creates all Considerations that will be linked to Options
// Options will only be selected if the Condition is true
*/
void AIConstructor_MCTS::DefineConsiderations()
{


}


/*
* DefineOptions links the Options with Considerations and Actions
*/
void AIConstructor_MCTS::DefineOptions()
{

	// Single Option for TicTacToe
	AddOptionByName("MakeMove", "ActionMakeMove");

}




