#include "InputMapper.h"


InputMapper::InputMapper(){
	clear();
}


InputMapper::~InputMapper(){
}

void InputMapper::dispatch(MappedInput& mappedInput){

	//perform mapping for each input context
	for (std::vector<InputContext>::iterator inputContextItr = inputContexts.begin(); inputContextItr != inputContexts.end(); ++inputContextItr) {
		int index = std::distance(inputContexts.begin(), inputContextItr);
		//if this context is active perform mapping
		if (activeContexts[index]) {
			//fill mappedInput from raw input using the contexts mapping
			inputContextItr->mapButtons(buttonStateDown, buttonStateWasDown, mappedInput);
			inputContextItr->mapAxes(axesState, axesStateValues, mappedInput);
		}
	}
	
}


void InputMapper::clear(){
	//don't use this because we want to keep track of state over frames
	//using swap
	memset(buttonStateDown, 0, InputRaw::getNumButtons() * sizeof(bool));
	memset(buttonStateWasDown, 0, InputRaw::getNumButtons() * sizeof(bool));
	memset(axesState, 0, InputRaw::getNumAxes() * sizeof(bool));
}

void InputMapper::swap(){
	//swap button state down to button state was down
	std::memcpy(buttonStateWasDown, buttonStateDown, InputRaw::getNumButtons() * sizeof(bool));
	//memset(buttonStateDown, 0, InputRaw::getNumButtons() * sizeof(bool));

}

void InputMapper::enableInputContext(std::string name){
	activeContexts[inputContextMap[name]] = true;
}

void InputMapper::disableInputContext(std::string name){
	activeContexts[inputContextMap[name]] = false;
}


void InputMapper::setButtonState(InputRaw::Buttons button, bool down, bool wasDown){
	int buttonIndex = InputRaw::getButtonIndex(button);
	buttonStateDown[buttonIndex] = down;;
	buttonStateWasDown[buttonIndex] = wasDown;
}

void InputMapper::addButton(InputRaw::Buttons button, bool down){
	int buttonIndex = InputRaw::getButtonIndex(button);
	buttonStateDown[buttonIndex] = down;
}

void InputMapper::addButtonDown(InputRaw::Buttons button){
	int buttonIndex = InputRaw::getButtonIndex(button);
	buttonStateDown[buttonIndex] = true;
}

void InputMapper::addButtonUp(InputRaw::Buttons button){
	int buttonIndex = InputRaw::getButtonIndex(button);
	buttonStateDown[buttonIndex] = false;
}

void InputMapper::addAxisValue(InputRaw::Axes axis, float value){
	int index = InputRaw::getAxisIndex(axis);
	axesState[index] = true;
	axesStateValues[index] = normalisedAxisValue(value);
}

float InputMapper::normalisedAxisValue(float value){
	return value;
}
