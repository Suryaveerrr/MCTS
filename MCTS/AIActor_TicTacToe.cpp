#include "AIActor_TicTacToe.h"
#include "AIBrain_TTT.h"
#include "GameController.h"
#include <iostream>

AIActor_TicTacToe::AIActor_TicTacToe()
{

	gameController = nullptr;
}




AIActor_TicTacToe::~AIActor_TicTacToe()
{

}



// **  AIActorBase overrides **



bool AIActor_TicTacToe::Init(std::string _id, AIConstructorBase& _constructor)
{
	actorID = _id;

	// create new brain
	brain =  std::make_unique<AIBrain_TTT>();

	brain->Init(_constructor, *this);



	return true;
}


/*
* SetGameContext will set the game controller context
*
 - AI decision making data should be on the blackboard, not accessed via the game controller

*/
void AIActor_TicTacToe::SetGameContext(GameController& _gameController) {

	gameController = &_gameController;
}



//  ** Game Loop **


/*
* Update will cause the AI Actor to use the brain to make a decision
*/
void AIActor_TicTacToe::Update(float _dt)
{
	brain->Update();
}




// ** Game Rules and Logic **


/*
* unsed, but can be used for initialisation
*/
void AIActor_TicTacToe::PreTransition()
{

}


void AIActor_TicTacToe::MakeMove(int _row, int _col)
{

	if (gameController == nullptr)
	{
		std::cout << "ERROR - gameController in AIActor_TicTacToe is null" << std::endl;
	}
	else
	{
		gameController->MakeMove(_row, _col);
	}

	}
