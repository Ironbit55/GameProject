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

	//loop through all buttons
	for (int i = SDL_CONTROLLER_BUTTON_A; i < SDL_CONTROLLER_BUTTON_MAX; i++){
		//should definetely be in enum bounds
		SDL_GameControllerButton sdlButton = static_cast<SDL_GameControllerButton>(i);
		InputRaw::Buttons rawInputButton;

		//check sdlButton has a mapping
		if(mapSdlControllerButtonToButton(sdlButton, rawInputButton)){
			//rawInputButton now the mapped value

			//add button down and up event based on state of the controller button
			if(controller.buttonDown(sdlButton)){
				//key repeat if button is held (means button was down previous frame)
				addButtonDown(actor, rawInputButton, controller.buttonHeld(sdlButton));
			}else if (controller.buttonReleased(sdlButton)) {
				addButtonUp(actor, rawInputButton);
			}
		}
	}

	//loop through all axes
	for (int i = SDL_CONTROLLER_AXIS_LEFTX; i < SDL_CONTROLLER_AXIS_MAX; i++) {
		SDL_GameControllerAxis sdlAxis = static_cast<SDL_GameControllerAxis>(i);
		InputRaw::Axes rawInputAxis;

		//check we have a mapping for sdl to raw input
		if(mapSdlControllerAxesToAxes(sdlAxis, rawInputAxis)){
			float normalisedAxisValue = normaliseSdlAxisValue(controller.axisValue(sdlAxis));

			//raw input axis now set to mapped value
			addAxisValue(actor, rawInputAxis, normalisedAxisValue);
		}
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

