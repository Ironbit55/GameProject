#include "GameController.h"
#include <iostream>


GameController::GameController(int index){
	gameController = SDL_GameControllerOpen(index);
	if (gameController == nullptr) {
		std::cout << "failed to open controller (" << index << ")!" << std::endl;
		return;
	}

	joystickId = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController));
}


GameController::~GameController(){

	if (gameController != nullptr) {
		//this is a bit dangerous... if we copy a game controller and the old one is deleted
		//it will close the controller the new game controller instance is linked to
		SDL_GameControllerClose(gameController);
	}
}

bool GameController::buttonEventBelongsToController(SDL_Event& e){
	if(e.type != SDL_CONTROLLERBUTTONDOWN){
		return false;
	}

	return e.cbutton.which == getJoystickId();
}
