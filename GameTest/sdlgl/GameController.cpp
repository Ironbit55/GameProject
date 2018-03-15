#include "GameController.h"
#include <iostream>
#include <string>
#include "../../nclgl/Keyboard.h"


GameController::GameController() : gameController(nullptr), controllerHaptic(nullptr), joystickId(-1), connected(false), haptic(false), deviceIndex(-1) {}


GameController::~GameController(){

	//if (gameController != nullptr) {
	//	//this is a bit dangerous... if we copy a game controller and the old one is deleted
	//	//it will close the controller the new game controller instance is linked to
	//	SDL_GameControllerClose(gameController);
	//}
}

bool GameController::open(int index){
	if(gameController != nullptr){
		std::cout << "failed to open controller (" << index << ")!: Cannot call open on already connected controller" << std::endl;
		return false;
	}
	gameController = SDL_GameControllerOpen(index);
	if (gameController == nullptr) {
		std::cout << "failed to open controller (" << index << ")!" << std::endl;
		return false;
	}
	deviceIndex = index;
	connected = true;
	SDL_Joystick *JoystickHandle = SDL_GameControllerGetJoystick(gameController);
	joystickId = SDL_JoystickInstanceID(JoystickHandle);

	//try and initialise sdl haptic capability
	
	controllerHaptic = SDL_HapticOpenFromJoystick(JoystickHandle);
	if(controllerHaptic != nullptr){
		//controller supports haptic
		haptic = true;

		//check basic rumble support if it doesn't then close it
		//in future will probably want to use custom haptic effects 
		if (SDL_HapticRumbleInit(controllerHaptic) != 0){	//returns 0 on success
			//does not support basic rumble
			SDL_HapticClose(controllerHaptic);
			controllerHaptic = nullptr;
			haptic = false;
		}
	}


	
	return true;
}

void GameController::close(){
	connected = false;
	if (gameController != nullptr) {
		SDL_GameControllerClose(gameController);
		gameController = nullptr;
	}
	if (controllerHaptic != nullptr) {
		SDL_HapticClose(controllerHaptic);
	}
	joystickId = -1;
	haptic = false;
}


bool GameController::rumble(){
	if(!haptic){
		std::cout << "failed to perforam rumble! controller does not support sdl haptic";
		return false;
	}

	SDL_HapticRumblePlay(controllerHaptic, 0.5f, 2000);
}

bool GameController::buttonEventBelongsToController(const SDL_Event& e){
	if(connected && (e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP)){
		return e.cbutton.which == getJoystickId();
	}

	return false;
}

bool GameController::deviceEventBelongsToController(const SDL_Event& e){
	if (!connected) {
		return false;
	}
	if(e.type == SDL_CONTROLLERDEVICEADDED){
		std::cout << "controller device added, device index: " + std::to_string(e.cdevice.which) + "\n";
		return false;
	}
	if (e.type == SDL_CONTROLLERDEVICEREMOVED || e.type == SDL_CONTROLLERDEVICEREMAPPED) {
		if (e.cdevice.which == joystickId) {
			std::cout << "controller device removed, device instance id: " + std::to_string(e.cdevice.which) + "\n";
			return true;
		}
	}

	return false;
}

void GameController::updateState(){
	if(!isConnected()){
		return;
	}

	//flip current key state to old state
	swapState();
	
	//loop through all buttons and update current state if pressed
	
	//https://wiki.libsdl.org/SDL_GameControllerButton
	//controller button a is 0
	for (int i = SDL_CONTROLLER_BUTTON_A; i < SDL_CONTROLLER_BUTTON_MAX; i++){
		//this should definetely be in bounds...right?
		SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(i);
		buttonState[button] = SDL_GameControllerGetButton(gameController, button);
	}

	//https://wiki.libsdl.org/SDL_GameControllerAxis
	for (int i = SDL_CONTROLLER_AXIS_LEFTX; i < SDL_CONTROLLER_AXIS_MAX; i++){
		SDL_GameControllerAxis axis = static_cast<SDL_GameControllerAxis>(i);
		axisState[axis] = SDL_GameControllerGetAxis(gameController, axis);
	}
}

void GameController::swapState(){
	//swap current key state into wasDown key state

	//use memcopy cus im cool
	memcpy(buttonWasDownState, buttonState, SDL_CONTROLLER_BUTTON_MAX * sizeof(bool));

	//we don't need to reset current key state as it will get overwritten next update
}

void GameController::clearState(){
	memset(buttonState, 0, SDL_CONTROLLER_BUTTON_MAX * sizeof(bool));
	memset(buttonWasDownState, 0, SDL_CONTROLLER_BUTTON_MAX * sizeof(bool));
}

bool GameController::buttonDown(const SDL_GameControllerButton button){
	return buttonState[button];
}

bool GameController::buttonHeld(const SDL_GameControllerButton button){
	return buttonState[button] && buttonWasDownState[button];
}

bool GameController::buttonTriggered(const SDL_GameControllerButton button){
	return buttonState[button] && !buttonWasDownState[button];
}

bool GameController::buttonReleased(const SDL_GameControllerButton button){
	return buttonWasDownState[button] && !buttonState[button];
}

int GameController::axisValue(SDL_GameControllerAxis axis){
	return axisState[axis];
}

void GameController::printButtonState(){
	std::cout << "controller " << joystickId << "state: ";
	for (int i = SDL_CONTROLLER_BUTTON_A; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
		std::cout << std::to_string(buttonState[i]);
	}
	std::cout << std::endl;
}
