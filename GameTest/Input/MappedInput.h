#pragma once
#include <vector>
#include "InputConstants.h"

class MappedInput
{
public:
	MappedInput();
	

	bool addAction(int actionId, bool state = true);
	bool addState(int stateId, bool state = true);
	bool addRange(int rangeId, float rangeValue, bool state = true);

	bool getAction(InputCooked::Actions actionId);
	bool getState(InputCooked::States stateId);
	bool getRange(InputCooked::Ranges rangeId, float& outRange);

	bool consumeAction(InputCooked::Actions actionId);
	bool consumeState(InputCooked::States stateId);
	bool consumeRange(InputCooked::Ranges rangeId, float& outRange);

	bool isEmpty() {
		return empty;
	}

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
	bool empty;
};

