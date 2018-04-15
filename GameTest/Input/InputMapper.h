#pragma once
#include <vector>
#include "InputContext.h"
#include <string>

class InputMapper{
public:
	InputMapper();
	virtual ~InputMapper();
	void dispatch(MappedInput& mappedInput);
	void clearMappedInput();
	void clear();
	void addInputContext(std::string name, InputContext& context) { inputContexts.push_back(context); }

	void addButtonDown(InputRaw::Buttons button, bool keyRepeat);
	void addButtonUp(InputRaw::Buttons button);

	//should convert raw value to normalised internal value based on some selected (ideally data driven) strategy
	//so could be unique per axis
	void addAxisValue(InputRaw::Axes axis, float value);

	virtual float normalisedAxisValue(float value);

protected:
	std::vector<InputContext> inputContexts;
	//MappedInput mappedInput;

	//ToDo: actually don't bother maintaing state here.
	//just add to mappedInput as we go?

	InputRaw::Buttons buttonState[InputRaw::getNumButtons()];			//is the button down?
	//the index for was down state will point to a button in the above array
	bool buttonStateWasDown[InputRaw::getNumButtons()];		//was the button down in the previous tick / poll
	bool buttonStateUp[InputRaw::getNumButtons()];
	int buttonStateTail;									//when we add a button state its added to the end of the button state
															//arrays, this records the tail for but button state and was down state
	
	InputRaw::Axes axesState[InputRaw::getNumAxes()];		//was the axes modified
	//index of axes values will point to axis in axesState array
	//so if looping through axes state, can use index to find value from axesValue
	float axesStateValues[InputRaw::getNumAxes()];				//what is axes value
	int axesStateTail;
};

