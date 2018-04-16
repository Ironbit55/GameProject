#include "SdlInputManager.h"



SdlInputManager::SdlInputManager(){
}


SdlInputManager::~SdlInputManager()
{
}

bool SdlInputManager::addSdlKeyMapping(SDL_Keycode sdlKey, InputRaw::Buttons rawInputButton) {
	return sdlKeyMap.insert(std::make_pair(sdlKey, rawInputButton)).second == true;
}

bool SdlInputManager::addSdlGameControllerButtonMapping(SDL_GameControllerButton sdlButton,
	InputRaw::Buttons rawInputButton){
	return sdlGameControllerButtonMap.insert(std::make_pair(sdlButton, rawInputButton)).second == true;
}

bool SdlInputManager::addSdlGameControllerAxesMapping(SDL_GameControllerAxis sdlAxes, InputRaw::Axes rawInputAxes){
	return sdlGameControllerAxesMap.insert(std::make_pair(sdlAxes, rawInputAxes)).second == true;
}

bool SdlInputManager::addSdlKeyEvent(InputActors actor, SDL_Event& e){
	if (e.type == SDL_KEYDOWN) {
		return addSdlKeyDown(actor, e.key.keysym.sym, e.key.repeat);

	} else if (e.type == SDL_KEYUP) {
		return addSdlKeyUp(actor, e.key.keysym.sym);
	}
	return false;
}

bool SdlInputManager::addSdlKeyUp(InputActors actor, SDL_Keycode keycode){
	InputRaw::Buttons buttonId;
	if (mapSdlKeyToButton(keycode, buttonId)) {
		addButtonUp(actor, buttonId);
		return true;
	}
	return false;
}

bool SdlInputManager::addSdlKeyDown(InputActors actor, SDL_Keycode keycode, bool keyRepeat){
	InputRaw::Buttons buttonId;
	if (mapSdlKeyToButton(keycode, buttonId)) {
		addButtonDown(actor, buttonId);
		return true;
	}
	return false;
}

void SdlInputManager::addSdlGameControllerState(InputActors actor, GameController& controller){

	//map buttons
	for (auto it = sdlGameControllerButtonMap.begin(); it != sdlGameControllerButtonMap.end(); ++it) {
		SDL_GameControllerButton sdlButton = it->first;
		InputRaw::Buttons rawInputButton = it->second;


		addButton(actor, rawInputButton, controller.getButton(sdlButton));
	}

	//map axes
	for (auto it = sdlGameControllerAxesMap.begin(); it != sdlGameControllerAxesMap.end(); ++it) {
		SDL_GameControllerAxis sdlAxis = it->first;
		InputRaw::Axes rawInputAxis = it->second;

		
		float normalisedAxisValue = normaliseSdlAxisValue(controller.getAxis(sdlAxis));
		addAxisValue(actor, rawInputAxis, normalisedAxisValue);
	}
}

void SdlInputManager::addSdlKeyboardState(InputActors actor){
	
	//grab current keyboard state from sdl
	const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
	for (auto it = sdlKeyMap.begin(); it != sdlKeyMap.end(); ++it) {
		SDL_Keycode sdlKey = it->first;
		InputRaw::Buttons rawInputButton = it->second;

		//SDL keyboardState uses scancodes as index,
		//scan codes are the physical keys wheras keycodes are values the keys are
		//mapped to. e.g USUALLY key (scancode) a maps to character (keycode) a but not neccesarily
		SDL_Scancode sdlScanCode = SDL_GetScancodeFromKey(sdlKey);
		
		addButton(actor, rawInputButton, keyboardState[sdlScanCode]);
	}
}

float SdlInputManager::normaliseSdlAxisValue(int value){
	int sdlRange = 2 * sdlAxisMaxValue;
	int internalRange = 2 * internalAxisMaxValue;

	float normalisedValue = (float)value / sdlAxisMaxValue;
	return normalisedValue;
}




//don't use these anymore

bool SdlInputManager::mapSdlKeyToButton(SDL_Keycode keycode, InputRaw::Buttons& outButtonId) {
	i = sdlKeyMap.find(keycode);

	if (i == sdlKeyMap.end()) {	/* Not found */
		return false;
	} else { //found
			 //return the mapped value
		outButtonId = i->second;
		return true;
	}
}

bool SdlInputManager::mapSdlControllerButtonToButton(SDL_GameControllerButton controllerButton,
	InputRaw::Buttons& outButtonId) {

	std::map<SDL_GameControllerButton, InputRaw::Buttons>::iterator i = sdlGameControllerButtonMap.find(controllerButton);

	if (i == sdlGameControllerButtonMap.end()) {	/* Not found */
		return false;
	} else { //found
			 //return the mapped value
		outButtonId = i->second;
		return true;
	}
}

bool SdlInputManager::mapSdlControllerAxesToAxes(SDL_GameControllerAxis controllerAxes, InputRaw::Axes& outAxesId) {

	std::map<SDL_GameControllerAxis, InputRaw::Axes>::iterator i = sdlGameControllerAxesMap.find(controllerAxes);
	if (i == sdlGameControllerAxesMap.end()) {	/* Not found */
		return false;
	} else { //found
			 //return the mapped value
		outAxesId = i->second;
		return true;
	}
}

