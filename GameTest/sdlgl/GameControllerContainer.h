/*
* Class: GameControllerContainer
* Author: Edward Curran (b5021040)
* Description: Manages multiple game controllers
* 
* handles game controller events to automatticaly
* open and close controllers on add and remove
*
* Updates the state of the game controllers
*/


#pragma once

#define MAX_CONTROLLERS 4
#include "GameController.h"

class GameControllerContainer
{
public:
	GameControllerContainer();
	~GameControllerContainer();

	//handles adding and removing controllers
	bool handleEvent(const SDL_Event& event);

	//updates state of controllers
	//need to call this for controllers to collect inputs
	void update();

	
	bool getController(int index, GameController& outController);
	bool connectedController(int index, GameController& outController);
protected:
	GameController controllers[MAX_CONTROLLERS];
};

