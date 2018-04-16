#include "InputMapper.h"


InputMapper::InputMapper(){
	clear();
}


InputMapper::~InputMapper(){
}

void InputMapper::dispatch(MappedInput& mappedInput){

	for (std::vector<InputContext>::iterator inputContextItr = inputContexts.begin(); inputContextItr != inputContexts.end(); ++inputContextItr) {
		int index = std::distance(inputContexts.begin(), inputContextItr);
		//if this context is active perform mapping
		if (activeContexts[index]) {
			inputContextItr->mapButtons(buttonStateDown, buttonStateWasDown, mappedInput);
			inputContextItr->mapAxes(axesState, axesStateValues, mappedInput);
		}
	}
	
}


void InputMapper::clear(){
	//this may not be neccesary if we always fill up zeroes anyway
	memset(buttonStateDown, 0, InputRaw::getNumButtons() * sizeof(bool));
	memset(buttonStateWasDown, 0, InputRaw::getNumButtons() * sizeof(bool));
	memset(axesState, 0, InputRaw::getNumAxes() * sizeof(bool));
}

void InputMapper::enableInputContext(std::string name){
	activeContexts[inputContextMap[name]] = true;
}

void InputMapper::disableInputContext(std::string name){
	activeContexts[inputContextMap[name]] = false;
}


void InputMapper::addButton(InputRaw::Buttons button, bool down, bool wasDown){
	int buttonIndex = InputRaw::getButtonIndex(button);
	buttonStateDown[buttonIndex] = down;;
	buttonStateWasDown[buttonIndex] = wasDown;
}

void InputMapper::addButtonDown(InputRaw::Buttons button, bool keyRepeat){
	int buttonIndex = InputRaw::getButtonIndex(button);
	buttonStateDown[buttonIndex] = true;
	buttonStateWasDown[buttonIndex] = keyRepeat;
	
}

void InputMapper::addButtonUp(InputRaw::Buttons button){
	int buttonIndex = InputRaw::getButtonIndex(button);
	buttonStateDown[buttonIndex] = false;
	buttonStateWasDown[buttonIndex] = true;
}

void InputMapper::addAxisValue(InputRaw::Axes axis, float value){
	int index = InputRaw::getAxisIndex(axis);
	axesState[index] = true;
	axesStateValues[index] = normalisedAxisValue(value);
}

float InputMapper::normalisedAxisValue(float value){
	return value;
}
