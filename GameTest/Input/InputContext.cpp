#include "InputContext.h"






InputContext::~InputContext(){}

void InputContext::setRawInputs(int rawInputs[MAX_INPUTS]){
	//copy passed in array into our member array
	//can't decide if this is the best way to do it or not
	//alternative is have member array be pointer or reference
	//but I mostly want the input context to handle its own memory footprint
	//can create a bucket of InputContexts etc
	for (int i = 0; i < MAX_INPUTS; i++){
		this->rawInputs[i] = rawInputs[i];
	}

}

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


bool InputContext::mapButtonInput(int buttonId, bool keyRepeat, MappedInput& mappedInput){
	if(!InputRaw::isButton(buttonId)){
		printf("could not map button, raw input with id: %i is not a valid button\n", buttonId);
		return false;
	}

	int mappedInputId = -1;
	bool success = false;

	//constructing this iterator will have some performance hit so try to avoid it...
	std::map<int, int>::iterator i;

	
	//check if button maps to state

	mappedInputId = getMappedInputId(buttonId, buttonToStateMap, i);
	if(mappedInputId != -1){
		//found
		//add the state this button maps to to our mapped input object
		mappedInput.addState(mappedInputId);
		success = true;
	}

	//action is only fired if button pressed and no keyRepeat
	//so only gets fired when button first pressed, or released?

	if(!keyRepeat){
		//check if button maps to action
		mappedInputId = getMappedInputId(buttonId, buttonToActionMap, i);
		if (mappedInputId != -1) {
			//found
			//add the action this button maps to to our mapped input object
			mappedInput.addAction(mappedInputId);
			success = true;
		}
	}

	return success;

}

bool InputContext::mapAxesInput(int axisId, float value, MappedInput& mappedInput){
	if(!InputRaw::isAxis(axisId)){
		printf("could not map axis, raw input with id: %i is not a valid axis\n", axisId);
		return false;
	}

	

	//constructing this iterator will have some performance hit so try to avoid it...
	std::map<int, int>::iterator i;

	int mappedInputId = getMappedInputId(axisId, axisToRangeMap, i);
	if (mappedInputId != -1) {
		//found
		//add the state this button maps to to our mapped input object
		mappedInput.addRange(mappedInputId, value);
		return true;
	}

	return false;;
}

