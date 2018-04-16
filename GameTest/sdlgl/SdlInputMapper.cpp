#include "SdlInputMapper.h"


SdlInputMapper::SdlInputMapper()
{
}


SdlInputMapper::~SdlInputMapper()
{
}

bool SdlInputMapper::addSdlKeyMapping(SDL_Keycode sdlKey, InputRaw::Buttons rawInputButton){
	return sdlKeyMap.insert(std::make_pair(sdlKey, rawInputButton)).second == true;
}

bool SdlInputMapper::addSdlKeyEvent(SDL_Event& e){
	if(e.type == SDL_KEYDOWN){
		return addSdlKeyDown(e.key.keysym.sym, e.key.repeat);
		
	} else if (e.type == SDL_KEYUP){
		return addSdlKeyUp(e.key.keysym.sym);
	}
	return false;
}

bool SdlInputMapper::addSdlKeyUp(SDL_Keycode keycode){
	InputRaw::Buttons buttonId;
	if(mapSdlKeyToButton(keycode, buttonId)){
		addButtonUp(buttonId);
		return true;
	}
	return false;
	
}

bool SdlInputMapper::addSdlKeyDown(SDL_Keycode keycode, bool keyRepeat){
	InputRaw::Buttons buttonId;
	if (mapSdlKeyToButton(keycode, buttonId)) {
		addButtonDown(buttonId);
		return true;
	}
	return false;
}

bool SdlInputMapper::addSdlGameControllerState(GameController& controller, int controllerIndex){
	//horrible hack to map from controller inputs
	//to raw inputs based on the controller index
	//the problem is we need some way to go from raw sdl controller input + which controller it is
	//to input raw

	//the proper solution is to have a unique map for each controller
	//but we aren't going to do that because the maps will be identical
	//and they don't need to be changed once all our raw input types are finilised

	//the hack we use is assume the 
	//1. raw input constants corrosponding to controller inputs are defined in a contigous block
	//2. the inputs are the same and in the same order for each controller

	//to do this use a single controller map to go from sdl controller input to generic raw input id
	//then shift raw input id up by multiples of the number of defined controller inputs 
	//depending on which controller this is
	return false;
}

bool SdlInputMapper::mapSdlKeyToButton(SDL_Keycode keycode, InputRaw::Buttons& outButtonId){
	i = sdlKeyMap.find(keycode);

	if (i == sdlKeyMap.end()) {	/* Not found */
		return false;
	} else { //found
			 //return the mapped value
		outButtonId = i->second;
		return true;
	}
}

bool SdlInputMapper::mapSdlControllerButtonToButton(SDL_GameControllerButton controllerButton,
	InputRaw::Buttons& outButtonId){

	std::map<SDL_GameControllerButton, InputRaw::Buttons>::iterator i = gameControllerButtonMap .find(controllerButton);

	if (i == gameControllerButtonMap.end()) {	/* Not found */
		return false;
	} else { //found
			 //return the mapped value
		outButtonId = i->second;
		return true;
	}
}

bool SdlInputMapper::mapSdlControllerAxesToAxes(SDL_GameControllerAxis controllerAxes, InputRaw::Axes& outAxesId) {

	std::map<SDL_GameControllerAxis, InputRaw::Axes>::iterator i = gameControllerAxesMap.find(controllerAxes);
	if (i == gameControllerAxesMap.end()) {	/* Not found */
		return false;
	} else { //found
			 //return the mapped value
		outAxesId = i->second;
		return true;
	}
}



