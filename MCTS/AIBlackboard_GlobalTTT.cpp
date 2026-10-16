#include "AIBlackboard_GlobalTTT.h"


AIBlackboard_GlobalTTT* AIBlackboard_GlobalTTT::instancePointer = nullptr;

AIBlackboard_GlobalTTT* AIBlackboard_GlobalTTT::getInstance()
{

	if (instancePointer == nullptr)
	{
		instancePointer = new AIBlackboard_GlobalTTT();
	}

	return(instancePointer);
}