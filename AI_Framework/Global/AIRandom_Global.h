#pragma once

#include <stdlib.h>
#include <time.h>
#include <random>

/*
CMP304/MAT501 AI Framework (2025)
*/


class AIRandom_Global
{
public:

	AIRandom_Global(const AIRandom_Global& obj) = delete;

	static AIRandom_Global* getInstance();

	double GetRandom01();
	int GetRandomValue(int _range);
	double GetRandomValue(float _range);


private:

	static AIRandom_Global* instancePointer;
	AIRandom_Global() { srand( (time(0))); }

	std::mt19937 rengine{ std::random_device{}() };
	std::uniform_real_distribution<double> rdist01{ 0.0, 1.0 };
};

