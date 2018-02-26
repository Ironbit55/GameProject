#pragma once
#include <SDL.h>
#include "SdlInputManager.h"
#include "GameControllerContainer.h"

class SdlInput
{
public:
	SdlInput(SdlInputManager& inputManager);
	~SdlInput();

	void update();
	void handleEvent(SDL_Event &e);

	GameControllerContainer& getControllerContainer() { return controllerContainer; }
private:
	SdlInputManager& inputManager;
	GameControllerContainer controllerContainer;
};

