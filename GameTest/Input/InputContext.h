/*
 * Author: Edward Curran (b15021040)
 * Class:InputContext
 * Description: holds a particular mapping from rawInput to mapped("cooked") input
 * used to fill a MappedInput instance with result of cooking raw inputs
 * in this context
 */

#pragma once
#include <map>
#include "InputConstants.h"
#include "MappedInput.h"
const int MAX_INPUTS = 32;
class InputContext
{
public:
	InputContext(bool active = false) : active(active){}
	~InputContext();

	bool loadFromFile(std::string fileName);

	void setRawInputs(int rawInputs[MAX_INPUTS]);

	/*
	 * detects the type of the raw and cooked input
	 * and adds to appropriate map automatically
	 */
	bool addMapping(int rawInputId, int cookedInputId);

	/*
	 * these provide ways to add mappings
	 */
	bool addActionMapping(InputRaw::Buttons button, InputCooked::Actions action);
	bool addStateMapping(InputRaw::Buttons button, InputCooked::States state);

	//TODO:should take a sensitivity value to apply to axis value to get range
	bool addRangeMapping(InputRaw::Axes axis, InputCooked::Ranges range);

	int getMappedInputId(int rawInputId, std::map<int, int>& map, std::map<int, int>::iterator& i);
	int getMappedAction(InputRaw::Buttons buttonId);
	int getMappedState(InputRaw::Buttons buttonId);
	int getMappedRange(InputRaw::Axes axisId);

	
	/*
	 * updates mappedInput according to whether this context has a mapping for the given raw input (button or axis)
	 * (and whether the raw input should trigger a mapped input)
	 */
	bool mapButtonInput(int buttonId, bool keyRepeat, MappedInput& mappedInput);

	//TODO:should apply sensitivity mapping to get final range value
	bool mapAxesInput(int axisId, float value, MappedInput& mappedInput);

protected:
	//could have two button to actions maps
	//one to trigger action on initial button down
	//and one to trigger on button release
	std::map<int, int> buttonToActionMap;
	std::map<int, int> buttonToStateMap;
	std::map<int, int> axisToRangeMap;

	std::map<InputRaw::Axes, float> axisToSensitivityMap;
	

	int rawInputs[MAX_INPUTS];
	

	bool active;
};

