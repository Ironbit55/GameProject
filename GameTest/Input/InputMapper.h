#pragma once
#include <vector>
#include "InputContext.h"
#include <string>

class InputMapper{
public:
	InputMapper();
	virtual ~InputMapper();
	void dispatch(MappedInput& mappedInput);
	
	void clear();
	void swap();
	void addInputContext(std::string name, InputContext& context){
		inputContexts.push_back(context);
		activeContexts.push_back(true);
		inputContextMap[name] = inputContexts.size() - 1;
	}
	
	void enableInputContext(std::string name);
	void disableInputContext(std::string name);

	void setButtonState(InputRaw::Buttons button, bool down, bool wasDown);
	void addButton(InputRaw::Buttons button, bool down);
	void addButtonDown(InputRaw::Buttons button);
	void addButtonUp(InputRaw::Buttons button);

	//should convert raw value to normalised internal value based on some selected (ideally data driven) strategy
	//so could be unique per axis
	void addAxisValue(InputRaw::Axes axis, float value);

	virtual float normalisedAxisValue(float value);

protected:
	std::map<std::string, int> inputContextMap;
	std::vector<InputContext> inputContexts;
	std::vector<bool> activeContexts;

	//ToDo: actually don't bother maintaing state here.
	//just add to mappedInput as we go?

	bool buttonStateDown[InputRaw::getNumButtons()];		//is the button down?
	bool buttonStateWasDown[InputRaw::getNumButtons()];	   //was the button down in the previous tick / poll
									
	
	bool axesState[InputRaw::getNumAxes()];				//was the axis modified
	float axesStateValues[InputRaw::getNumAxes()];		//what is axis value
	
};

