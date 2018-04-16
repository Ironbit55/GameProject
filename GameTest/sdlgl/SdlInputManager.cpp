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
		addButtonDown(actor, buttonId, keyRepeat);
		return true;
	}
	return false;
}

void SdlInputManager::addSdlGameControllerState(InputActors actor, GameController& controller){

	//this is a weird way of doing it
	//if we got access to the raw state data of the controller we could just
	//map that directly into button state...

	for (auto it = sdlGameControllerButtonMap.begin(); it != sdlGameControllerButtonMap.end(); ++it) {
		SDL_GameControllerButton sdlButton = it->first;
		InputRaw::Buttons rawInputButton = it->second;

		addButton(actor, rawInputButton, controller.buttonDown(sdlButton), controller.buttonHeld(sdlButton));
	}

	for (auto it = sdlGameControllerAxesMap.begin(); it != sdlGameControllerAxesMap.end(); ++it) {
		SDL_GameControllerAxis sdlAxis = it->first;
		InputRaw::Axes rawInputAxis = it->second;

		float normalisedAxisValue = normaliseSdlAxisValue(controller.axisValue(sdlAxis));
		addAxisValue(actor, rawInputAxis, normalisedAxisValue);
	}
}

float SdlInputManager::normaliseSdlAxisValue(int value){
	int sdlRange = 2 * sdlAxisMaxValue;
	int internalRange = 2 * internalAxisMaxValue;

	float normalisedValue = (float)value / sdlAxisMaxValue;
	return normalisedValue;
}

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

