/*
 * Author: Edward Curran (b15021040)
 * Class:InputContext
 * Description: holds a particular mapping from rawInput to mapped("cooked") input
 * used to fill a MappedInput instance with result of cooking raw inputs
 * in this context
 */
struct MappedInputPair{
	MappedInputPair(int rawInputId, int cookedInputId) : rawInputId(rawInputId), cookedInputId(cookedInputId){}

	int rawInputId;
	int cookedInputId;
};
#pragma once
#include <map>
#include "InputConstants.h"
#include "MappedInput.h"
const int MAX_INPUTS = 32;
class InputContext
{
public:
	InputContext(bool active = false) : active(active), axisDeadzone(0.1f){}
	~InputContext();

	bool loadFromFile(std::string fileName);


	/*
	 * detects the type of the raw and cooked input
	 * and adds to appropriate map automatically
	 */
	bool addMapping(int rawInputId, int cookedInputId);

	/*
	please give the positiove value or we're gunno have a bad time
	*/
	void addDeadzone(float deadzone) {
		this->axisDeadzone = deadzone;
	}

	/*
	 * these provide ways to add mappings
	 */
	bool addActionMapping(InputRaw::Buttons button, InputCooked::Actions action);
	bool addStateMapping(InputRaw::Buttons button, InputCooked::States state);

	//TODO:should take a sensitivity value to apply to axis value to get range
	bool addRangeMapping(InputRaw::Axes axis, InputCooked::Ranges range);

	int getMappedInputId(int rawInputId, std::map<int, int>& map, std::map<int, int>::iterator& i);

	
	/*
	 * updates mappedInput according to whether this context has a mapping for the given raw input (button or axis)
	 * (and whether the raw input should trigger a mapped input)
	 */
	bool mapButtonInput(InputRaw::Buttons button, bool buttonDown, bool buttonWasDown, MappedInput& mappedInput);
	void mapButtons(bool* buttonDown, bool* buttonWasDown, MappedInput& mappedInput);

	//TODO:should apply sensitivity mapping to get final range value
	/*
	should be given a normalized axis value between -1 and 1 
	this will apply deadzone and sensitivity to ti
	*/
	bool mapAxisInput(InputRaw::Axes axis, float value, MappedInput& mappedInput);
	void mapAxes(bool* axes, float* values, MappedInput& mappedInput);

protected:
	//could have two button to actions maps
	//one to trigger action on initial button down
	//and one to trigger on button release

	//originally i used maps, but then I changes the design a lot
	//to try and follow data oriented design
	//so I was only using the map to iterate through it
	//then i realised map isn't stored in contingious block
	//which is bad on cache for iteration, would be better to use
	//use something like a vector

	//std::map<int, int> buttonToActionMap;
	//std::map<int, int> buttonToStateMap;
	//std::map<int, int> axisToRangeMap;

	
	std::vector<MappedInputPair> buttonToActionPairs;
	std::vector<MappedInputPair> buttonToStatePairs;
	std::vector<MappedInputPair> axisToRangePairs;

	
	std::map<InputRaw::Axes, float> axisToSensitivityMap;
	std::map<InputRaw::Axes, float> axisToDeadzoneMap;

	//im just gunno use one deadzone for all axes
	//cus when are you gunno wanna have different deadzones per axis 
	//that would be dumb
	float axisDeadzone;
	
	
	bool active;
};

