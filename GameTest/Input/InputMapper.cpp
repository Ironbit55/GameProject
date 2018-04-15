#include "InputMapper.h"


InputMapper::InputMapper(){
	clear();
}


InputMapper::~InputMapper(){
}

void InputMapper::dispatch(MappedInput& mappedInput){

	for (int i = 0; i < buttonStateTail; i++) {
		InputRaw::Buttons button = buttonState[i];

		for (std::vector<InputContext>::iterator inputContextItr = inputContexts.begin(); inputContextItr != inputContexts.end(); ++inputContextItr) {
			bool mapped = inputContextItr->mapButtonInput(button, buttonStateWasDown[i], mappedInput);
			if(mapped){
				//don't map the same button twice
				break;
			}
		}
	}

	for (int i = 0; i < axesStateTail; i++) {
		InputRaw::Axes axis = axesState[i];

		for (std::vector<InputContext>::iterator inputContextItr = inputContexts.begin(); inputContextItr != inputContexts.end(); ++inputContextItr) {
			bool mapped = inputContextItr->mapAxisInput(axis, axesStateValues[i], mappedInput);
			if (mapped) {
				//don't map the same axis twice
				break;
			}
		}
	}
}

void InputMapper::clear(){
	//start filling up button and axes state from 0 again
	//this means we don't even have to clear state it will just be overwritten
	//and we only ever read state from tail down so we won't read any dirty data
	buttonStateTail = 0;
	axesStateTail = 0;
}

void InputMapper::addButtonDown(InputRaw::Buttons button, bool keyRepeat){
	buttonState[buttonStateTail] = button;
	//button was down should be false as key repeat is false
	buttonStateWasDown[buttonStateTail] = keyRepeat;
	buttonStateUp[buttonStateTail] = false;;
	buttonStateTail++;

}

void InputMapper::addButtonUp(InputRaw::Buttons button){
	buttonState[buttonStateTail] = button;
	buttonStateWasDown[buttonStateTail] = false;
	buttonStateUp[buttonStateTail] = true;
	buttonStateTail++;
}

void InputMapper::addAxisValue(InputRaw::Axes axis, float value){
	float normalisedValue = normalisedAxisValue(value);
	//normalise value?
	axesState[axesStateTail] = axis;
	axesStateValues[axesStateTail] = normalisedValue;
	axesStateTail++;
}

float InputMapper::normalisedAxisValue(float value){
	return value;
}
