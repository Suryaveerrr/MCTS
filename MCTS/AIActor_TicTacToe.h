#pragma once
#include "../AI_Framework/framework.h"

#ifndef AI_ACTOR_TTT_H
#define AI_ACTOR_TTT_H

class GameController;


class AIActor_TicTacToe :
    public AIActorBase
{
public:
	AIActor_TicTacToe();

	~AIActor_TicTacToe();

	// AIActorBase overrides
	bool Init(std::string _id, AIConstructorBase& _constructor) override;
	void SetGameContext(GameController& _gameController);
	void Update(float _dt) override;
	void PreTransition() override;//State Transition Functions

	void MakeMove(int _row, int _col);

	GameController* gameController;

};

#endif // !AI_ACTOR_TTT_H