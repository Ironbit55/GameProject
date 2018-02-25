#include "GameControllerContainer.h"
#include <string>
#include <iostream>


GameControllerContainer::GameControllerContainer()
{
}


GameControllerContainer::~GameControllerContainer()
{
}

bool GameControllerContainer::handleEvent(const SDL_Event& e){
	switch (e.type) {
		case SDL_CONTROLLERDEVICEADDED: {
			std::cout << "controller device added, device index: " + std::to_string(e.cdevice.which) + "\n";
			if (e.cdevice.which < MAX_CONTROLLERS) {
				controllers[e.cdevice.which].open(e.cdevice.which);
				return true;
			}
			break;
		}
		case SDL_CONTROLLERDEVICEREMOVED: {
			for (int i = 0; i < MAX_CONTROLLERS; i++){
				if(controllers[i].deviceEventBelongsToController(e)){
					controllers[i].close();
					return true;
				}
			}
			break;
		}
	}

	return false;
}

void GameControllerContainer::update(){
	for (int i = 0; i < MAX_CONTROLLERS; i++) {
		controllers[i].updateState();
	}
}

GameController& GameControllerContainer::getController(int index){
	//could only return controller if connected?
	return controllers[index];
}
