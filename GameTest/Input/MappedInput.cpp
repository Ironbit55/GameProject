#include "MappedInput.h"
#include <iostream>


MappedInput::MappedInput(){
	clear();
}


bool MappedInput::addAction(int actionId){
	if(!InputCooked::isAction(actionId)){
		printf("could not add action input to MappedInput, cookedInput id: %i is not a valid action\n", actionId);
		return false;
	}
	//gets an index between 0 and max number of actions
	int actionIndex = actionId - InputCooked::ACTION_ID;
	if(actions[actionIndex]){
		printf("action input with id: %i already exists in MappedInput\n", actionId);
		return false;
	}

	empty = false;
	actions[actionIndex] = true;
	return true;
}

bool MappedInput::addState(int stateId){
	if (!InputCooked::isState(stateId)) {
		printf("could not add state input to MappedInput, cookedInput id: %i is not a valid state\n", stateId);
		return false;
	}
	//gets an index between 0 and max number of actions
	int stateIndex = stateId - InputCooked::STATE_ID;
	if (states[stateIndex]) {
		printf("state input with id: %i already exists in MappedInput\n", stateId);
		return false;
	}

	empty = false;
	states[stateIndex] = true;
	return true;
}

bool MappedInput::addRange(int rangeId, float rangeValue){
	if (!InputCooked::isRange(rangeId)) {
		printf("could not add range input to MappedInput, cookedInput id: %i is not a valid range\n", rangeId);
		return false;
	}
	//gets an index between 0 and max number of actions
	int rangeIndex = rangeId - InputCooked::RANGE_ID;
	if (actions[rangeIndex]) {
		printf("range input with id: %i already exists in MappedInput\n", rangeId);
		return false;
	}

	empty = false;
	ranges[rangeIndex] = true;
	rangeValues[rangeIndex] = rangeValue;
	return true;
}

bool MappedInput::getAction(InputCooked::Actions actionId){
	return actions[actionId - InputCooked::ACTION_ID];
}

bool MappedInput::getState(InputCooked::States stateId){
	return states[stateId - InputCooked::STATE_ID];
}

bool MappedInput::getRange(InputCooked::Ranges rangeId, float& outRange){
	int rangeIndex = rangeId - InputCooked::RANGE_ID;
	if(ranges[rangeIndex]){
		outRange = rangeValues[rangeIndex];
		return true;
	}

	return false;
}

bool MappedInput::consumeAction(InputCooked::Actions actionId){
	bool result = actions[actionId - InputCooked::ACTION_ID];
	//destroy action
	actions[actionId - InputCooked::ACTION_ID] = false;
	return result;
}

bool MappedInput::consumeState(InputCooked::States stateId){
	bool result = states[stateId - InputCooked::STATE_ID];
	states[stateId - InputCooked::STATE_ID] = false;
	return result;
}

bool MappedInput::consumeRange(InputCooked::Ranges rangeId, float & outRange){
	int rangeIndex = rangeId - InputCooked::RANGE_ID;
	if (ranges[rangeIndex]) {
		outRange = rangeValues[rangeIndex];

		//destroy range
		ranges[rangeIndex] = false;
		rangeValues[rangeIndex] = 0.0f;

		return true;
	}

	return false;
}

void MappedInput::clear(){
	//memset is cross platform right?
	memset(actions, 0, InputCooked::getNumActions() * sizeof(bool));
	memset(states, 0, InputCooked::getNumStates() * sizeof(bool));
	memset(ranges, 0, InputCooked::getNumRanges() * sizeof(bool));

	memset(rangeValues, 0, InputCooked::getNumRanges() * sizeof(float));
	empty = true;

}
