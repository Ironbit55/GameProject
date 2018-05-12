/*
* Author: Ed Curran (b15021040)
* Class: SdlInput
* Description: pretty much the final layer of the input system
* it collects raw sdl keyboard and game controller input and pushes
* it into the provided SdlInputManager instance
* 
* It also handles updating and dispatching the InputManager
*
*
*/

#pragma once
#include <SDL.h>
#include "SdlInputManager.h"
#include "GameControllerContainer.h"
#include "../GameTest/MessageReceiver.h"

struct ControllerRumbleMsgData {
	InputActors inputActor;
};

class SdlInput : MessageReceiver
{
public:
	SdlInput(SdlInputManager& inputManager);
	~SdlInput();

	void loadMappings();

	void update();
	void handleEvent(SDL_Event &e);

	void onRumbleMessage(Message msg);

	GameControllerContainer& getControllerContainer() { return controllerContainer; }
private:
	SdlInputManager& inputManager;
	GameControllerContainer controllerContainer;
};

