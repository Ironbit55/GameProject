#include "InputContext.h"






InputContext::~InputContext(){}

bool InputContext::addMapping(int rawInputId, int cookedInputId){
	if(InputRaw::isButton(rawInputId)){
		if(InputCooked::isAction(cookedInputId)){
			//add to buttonToAction map
			return buttonToActionMap.insert(std::make_pair(rawInputId, cookedInputId)).second;
		}
		if(InputCooked::isState(cookedInputId)){
			//add to buttonToState map
			return buttonToStateMap.insert(std::make_pair(rawInputId, cookedInputId)).second;
		}

		//shouldn't have any other combination with button
	}
	if(InputRaw::isAxis(rawInputId)){
		if(InputCooked::isRange(cookedInputId)){
			//add to axisToRange map
			return axisToRangeMap.insert(std::make_pair(rawInputId, cookedInputId)).second;
		}

		//shouldn't be any more combinations
	}
	printf("raw input with id: %i and cooked input with id: %i is not a valid mapping\n", rawInputId, cookedInputId);
	return false;
}

bool InputContext::addActionMapping(InputRaw::Buttons button, InputCooked::Actions action){
	//should return false if can't insert (beccause duplicate value)
	return buttonToActionMap.insert(std::make_pair(button, action)).second;
}

bool InputContext::addStateMapping(InputRaw::Buttons button, InputCooked::States state){
	//should return false if can't insert (beccause duplicate value)
	return buttonToStateMap.insert(std::make_pair(button, state)).second;
}

bool InputContext::addRangeMapping(InputRaw::Axes axis, InputCooked::Ranges range){
	//should return false if can't insert (beccause duplicate value)
	return axisToRangeMap.insert(std::make_pair(axis, range)).second;
}

int InputContext::getMappedInputId(int rawInputId, std::map<int, int>& map, std::map<int, int>::iterator& i){
	i = map.find(rawInputId);

	if (i == map.end()){	/* Not found */
		return -1;
	}else { //found
		//return the mapped value
		return i->second;
	}
}

//int InputContext::getMappedAction(InputRaw::Buttons buttonId) {
//	std::map<InputRaw::Buttons, InputCooked::Actions>::iterator& it = buttonToActionMap.find(buttonId);
//
//	if (it == buttonToActionMap.end()) {	/* Not found */
//		return -1;
//	}
//	
//	return it->second;
//	
//
//}


bool InputContext::mapButtonInput(InputRaw::Buttons button, bool buttonDown, bool buttonWasDown, MappedInput& mappedInput){
	bool success = false;
	int mappedInputId = -1;


	//constructing this iterator will have some performance hit so try to avoid it...
	std::map<int, int>::iterator i;

	//check if button maps to state
	mappedInputId = getMappedInputId(button, buttonToStateMap, i);
	if(mappedInputId != -1){
		//found
		//add the state this button maps to to our mapped input object
		mappedInput.addState(mappedInputId, buttonDown);
		success = true;
	}

	//check if button maps to action
	mappedInputId = getMappedInputId(button, buttonToActionMap, i);
	if (mappedInputId != -1) {
		//found

		//action is only fired if button pressed and no keyRepeat
		//so only gets fired when button first pressed, what about released?
		//add the action this button maps to to our mapped input object
		mappedInput.addAction(mappedInputId, (buttonDown && !buttonWasDown));
		//could map button up (buttonWasDown && !buttonDown)
		//to action here
		success = true;
	}
	

	return success;

}

void InputContext::mapButtons(bool* buttonStateDown, bool* buttonStateWasDown, MappedInput& mappedInput){
	
	//should check for button up event first

	//map buttons to actions
	for (auto it = buttonToActionMap.begin(); it != buttonToActionMap.end(); ++it) {
		int buttonIndex = it->first - InputRaw::BUTTON_ID;
		int actionId = it->second;

		//button can be mapped to action if button just pressed (buttonDown, wasUp)
		bool map = (buttonStateDown[buttonIndex] && !buttonStateWasDown[buttonIndex]);
		mappedInput.addAction(actionId, map);

		//this disables button down events triggering for this button
		//for other input contexts in this frame
		//although it means button up could trigger...
		buttonStateWasDown[buttonIndex] = (buttonStateWasDown[buttonIndex] && map);

	}

	//map buttons to states
	for (auto it = buttonToStateMap.begin(); it != buttonToStateMap.end(); ++it) {
		int buttonIndex = it->first - InputRaw::BUTTON_ID;
		int stateId = it->second;
		
		//button can be mapped to state if button is down
		bool map = buttonStateDown[buttonIndex];
		mappedInput.addState(stateId, map);

		//button can only be mapped once. so if its been mapped before can't map it again
		buttonStateDown[buttonIndex] = (buttonStateDown[buttonIndex] && map);
	}
}

void InputContext::mapAxes(bool* axisState, float* axisValues, MappedInput& mappedInput) {
	for (auto it = axisToRangeMap.begin(); it != axisToRangeMap.end(); ++it) {
		int axisIndex = it->first - InputRaw::AXIS_ID;
		int rangeId = it->second;

		//apply deadzone to axis value
		bool inDeadzone = (axisValues[axisIndex] > -axisDeadzone && axisValues[axisIndex] < axisDeadzone);
		bool map = axisState[axisIndex] && !inDeadzone;
		
		//add the state this button maps to to our mapped input object
		mappedInput.addRange(rangeId, axisValues[axisIndex], map);
	}
}

bool InputContext::mapAxisInput(InputRaw::Axes axis, float value, MappedInput& mappedInput){

	//constructing this iterator will have some performance hit so try to avoid it...
	std::map<int, int>::iterator i;

	int mappedInputId = getMappedInputId(axis, axisToRangeMap, i);
	if (mappedInputId != -1) {
		//apply deadzone to axis value
		if (value > -axisDeadzone && value < axisDeadzone) {
			//within deadzone so
			//we don't pass it to mapped input but mapping didn't fail
			return true;
		}
		

		//add the state this button maps to to our mapped input object
		mappedInput.addRange(mappedInputId, value);
		return true;
	}

	return false;;
}


