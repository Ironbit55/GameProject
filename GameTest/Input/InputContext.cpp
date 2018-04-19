#include "InputContext.h"
#include <iostream>


InputContext::~InputContext(){}

bool InputContext::addMapping(int rawInputId, int cookedInputId){
	if(InputRaw::isButton(rawInputId)){
		if(InputCooked::isAction(cookedInputId)){
			//add to buttonToAction map
			buttonToActionPairs.push_back(MappedInputPair(rawInputId, cookedInputId));
			//return buttonToActionMap.insert(std::make_pair(rawInputId, cookedInputId)).second;
			return true;
		}
		if(InputCooked::isState(cookedInputId)){
			//add to buttonToState map
			buttonToStatePairs.push_back(MappedInputPair(rawInputId, cookedInputId));
			//return buttonToStateMap.insert(std::make_pair(rawInputId, cookedInputId)).second;
			return true;
		}

		//shouldn't have any other combination with button
	}
	if(InputRaw::isAxis(rawInputId)){
		if(InputCooked::isRange(cookedInputId)){
			//add to axisToRange map
			axisToRangePairs.push_back(MappedInputPair(rawInputId, cookedInputId));
			//return axisToRangeMap.insert(std::make_pair(rawInputId, cookedInputId)).second;
			return true;
		}

		//shouldn't be any more combinations
	}
	printf("raw input with id: %i and cooked input with id: %i is not a valid mapping\n", rawInputId, cookedInputId);
	return false;
}

bool InputContext::addActionMapping(InputRaw::Buttons button, InputCooked::Actions action){
	//should return false if can't insert (beccause duplicate value)
	buttonToActionPairs.push_back(MappedInputPair(button, action));
	//return buttonToActionMap.insert(std::make_pair(button, action)).second;
	return true;
}

bool InputContext::addStateMapping(InputRaw::Buttons button, InputCooked::States state){
	//should return false if can't insert (beccause duplicate value)
	buttonToStatePairs.push_back(MappedInputPair(button, state));
	return true;
	//return buttonToStateMap.insert(std::make_pair(button, state)).second;
}

bool InputContext::addRangeMapping(InputRaw::Axes axis, InputCooked::Ranges range){
	//should return false if can't insert (beccause duplicate value)
	axisToRangePairs.push_back(MappedInputPair(axis, range));
	return true;
	//return axisToRangeMap.insert(std::make_pair(axis, range)).second;
}


void InputContext::mapButtons(bool* buttonStateDown, bool* buttonStateWasDown, MappedInput& mappedInput){
	
	//should check for button up event first

	//map buttons to actions
	//map collection isn't contigious :/
	//if all we need is to iterate like this could use containing button - action pair
	//would be more cache friendly
	for (auto it = buttonToActionPairs.begin(); it != buttonToActionPairs.end(); ++it) {
		int buttonIndex = it->rawInputId -InputRaw::BUTTON_ID;
		int actionId = it->cookedInputId;

		//button can be mapped to action if button just pressed (buttonDown, wasUp)
		bool map = (buttonStateDown[buttonIndex] && !buttonStateWasDown[buttonIndex]);
		mappedInput.addAction(actionId, map);


		//hacky way to disable button down events triggering for this button
		//for other input contexts in this frame
		//although it means button up could trigger...
		buttonStateWasDown[buttonIndex] = (buttonStateWasDown[buttonIndex] && map);

	}


	//map buttons to states
	for (auto it = buttonToStatePairs.begin(); it != buttonToStatePairs.end(); ++it) {
		int buttonIndex = it->rawInputId - InputRaw::BUTTON_ID;
		int stateId = it->cookedInputId;
		
		//button can be mapped to state if button is down
		bool map = buttonStateDown[buttonIndex];
		mappedInput.addState(stateId, map);

		//button can only be mapped once. so if its been mapped before can't map it again
		buttonStateDown[buttonIndex] = (buttonStateDown[buttonIndex] && map);
	}
}

void InputContext::mapAxes(bool* axisState, float* axisValues, MappedInput& mappedInput) {
	for (auto it = axisToRangePairs.begin(); it != axisToRangePairs.end(); ++it) {
		int axisIndex = it->rawInputId - InputRaw::AXIS_ID;
		int rangeId = it->cookedInputId;

		//apply deadzone to axis value
		bool inDeadzone = (axisValues[axisIndex] > -axisDeadzone && axisValues[axisIndex] < axisDeadzone);
		bool map = axisState[axisIndex] && !inDeadzone;
		
		//add the state this button maps to to our mapped input object
		mappedInput.addRange(rangeId, axisValues[axisIndex], map);
	}
}




