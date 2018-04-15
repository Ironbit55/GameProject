#include "InputMapper.h"


InputMapper::InputMapper()
{
}


InputMapper::~InputMapper()
{
}

void InputMapper::dispatch(MappedInput& mappedInput){
	for (std::vector<InputContext>::iterator it = inputContexts.begin(); it != inputContexts.end(); ++it) {
		
		for (int i = 0; i < InputRaw::getNumButtons(); i++) {
			int buttonId = i + InputRaw::BUTTON_ID;
			if (buttonState[i]) {

				if (it->mapButtonInput(buttonId, buttonWasDownState[i], mappedInput)) {
					//this input has been handled
					buttonState[i] = false;
				}
			}

			//could map a key released event here
		}
		for (int i = 0; i < InputRaw::getNumAxes(); i++) {
			int axisId = i + InputRaw::AXIS_ID;
			if (axesState[i]) {
				if (it->mapAxesInput(axisId, axesValues[i], mappedInput)) {
					//input has been handled.
					axesState[i] = false;
					axesValues[i] = 0.0f;
				}
			}
		}
	}
}

void InputMapper::clear(){
	memset(buttonState, 0, InputRaw::getNumButtons() * sizeof(bool));
	memset(buttonWasDownState, 0, InputRaw::getNumButtons() * sizeof(bool));
	memset(axesState, 0, InputRaw::getNumAxes() * sizeof(bool));
	memset(axesValues, 0, InputRaw::getNumAxes() * sizeof(float));
	//fill up button and axes state from 0 again
	//this means we don't even have to clear dtate it will just be overwritten
	buttonStateTail = 0;
	axesStateTail = 0;
}

void InputMapper::addButtonDown(InputRaw::Buttons button, bool keyRepeat){
	int buttonIndex = button - InputRaw::BUTTON_ID;

	buttonState[buttonIndex] = true;
	if(keyRepeat){
		buttonWasDownState[buttonIndex] = true;
	}else{
		//button was down should be false as key repeat is false
		//reason its true is because
		//when a button is released the button was down state remains true
		//this is so we can detect key release event (key was down, not down now)
		//solution is we either clear our key state every frame
		//or we can set it back to false here meaning button down
		//still triggers actions (down now, not was down) correctly
		buttonWasDownState[buttonIndex] = false;
	}
}

void InputMapper::addButtonUp(InputRaw::Buttons button){
	int buttonIndex = button - InputRaw::BUTTON_ID;

	buttonState[buttonIndex] = false;
	buttonWasDownState[buttonIndex] = true;
}

void InputMapper::addAxisValue(InputRaw::Axes axis, float value){
	int axisIndex = axis - InputRaw::AXIS_ID;
	float normalisedValue = normalisedAxisValue(value);
	//normalise value?
	axesState[axisIndex] = true;
	axesValues[axisIndex] = normalisedValue;
}

float InputMapper::normalisedAxisValue(float value){
	return value;
}
