#pragma once
#include "..\AI_Framework\Global\AIBlackboard_Global.h"
#include "GameState.h"
class AIBlackboard_GlobalTTT 
{
public:

	AIBlackboard_GlobalTTT(const AIBlackboard_GlobalTTT& obj) = delete;

	static AIBlackboard_GlobalTTT* getInstance();



	template<typename T>
	void AddNewValue(std::string name, T data)
	{
		GetMap<T>()[name] = data;
	}

	template<typename T>
	T GetValue(std::string name)
	{
		return GetMap<T>().at(name);
	}

	template<typename T>
	T GetAndDeleteValue(std::string name)
	{
		T _val = GetMap<T>().at(name);
		GetMap<T>().erase(name);
		return _val;
	}

	template<typename T>
	void EditValue(std::string name, T newValue)
	{
		GetMap<T>()[name] = newValue;
	}

	template<typename T>
	void DeleteValue(std::string name)
	{
		GetMap<T>().erase(name);
	}

	template<typename T>
	bool HasValue(std::string name)
	{
		return GetMap<T>().contains(name);

	}



protected:


	template<typename T>
	auto& GetMap() { return GetMap(static_cast<T*>(nullptr)); }

	// allowed map types are defined below
	std::map<std::string, float>& GetMap(float*) { return datamapFloat; }
	std::map<std::string, int>& GetMap(int*) { return datamapInt; }
	std::map<std::string, std::string>& GetMap(std::string*) { return datamapStr; }
	std::map<std::string, AIMath_Global::Vector2f>& GetMap(AIMath_Global::Vector2f*) { return datamapVec2f; }
	std::map<std::string, std::vector<int>>& GetMap(std::vector<int>*) { return datamapIntVec; }
	std::map<std::string, GameState>& GetMap(GameState*) { return datamapGameState; }

	std::map<std::string, float> datamapFloat;
	std::map<std::string, AIMath_Global::Vector2f> datamapVec2f;
	std::map<std::string, std::vector<int> > datamapIntVec;
	std::map<std::string, std::string > datamapStr;
	std::map<std::string, int> datamapInt;
	std::map<std::string, GameState> datamapGameState;

private:

	static AIBlackboard_GlobalTTT* instancePointer;
	AIBlackboard_GlobalTTT() {}

	// allowed map types are defined below
	



};

