/*
 * Class: GameController
 * Author: Edward Curran (b5021040)
 * Description: Manages one instance of an sdl game controller
 */

#pragma once
#include <SDL.h>

class GameController
{
public:
	GameController(int index);
	~GameController();

	SDL_GameController* getSdlController() const{
		return gameController;
	};
	SDL_JoystickID getJoystickId() const{ 
		return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController)); 
	}

	bool eventBelongsToController(SDL_Event& e);
	bool buttonEventBelongsToController(SDL_Event& e);
private:
	SDL_GameController *gameController;
	//under the hood sdl game controllers are treated as joysticks. I think actually more than one joystick per controller
	//im slightly nervous that the underlying joystick id of a controller may change
	//eg after removed...
	SDL_JoystickID joystickId;
};

