#pragma once
#include <vector>
#include "InputConstants.h"

class MappedInput
{
public:
	MappedInput();
	

	bool addAction(int actionId);
	bool addState(int stateId);
	bool addRange(int rangeId, float rangeValue);

	bool getAction(InputCooked::Actions actionId);
	bool getState(InputCooked::States stateId);
	bool getRange(InputCooked::Ranges rangeId, float& outRange);

	void clear();
protected:
	//std::vector<int> actions;
	//std::vector<int> states;
	//std::vector<int> ranges;
	//std::vector<float> rangeValues;
	
	bool actions[InputCooked::getNumActions()];
	bool states[InputCooked::getNumStates()];
	bool ranges[InputCooked::getNumRanges()];
	float rangeValues[InputCooked::getNumRanges()];
};

