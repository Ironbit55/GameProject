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

	bool buttonState[InputRaw::getNumButtons()];			//is the button down?
	bool buttonWasDownState[InputRaw::getNumButtons()];		//was the button down in the previous tick / poll
	int buttonStateTail;									//when we add a button state its added to the end of the button state
															//arrays, this records the tail for but button state and was down state
	float axesValues[InputRaw::getNumAxes()];				//what is axes value
	bool axesState[InputRaw::getNumAxes()];					//was the axes modified
	int axesStateTail;
};

