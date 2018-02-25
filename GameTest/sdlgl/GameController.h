/*
 * Class: GameController
 * Author: Edward Curran (b5021040)
 * Description: Manages one instance of an sdl game controller
 * 
 * Sdl controller input events only occur on initial button down and on button up
 * with no key repeat info
 * as a result we need to maintain the state of the controller and store it
 * so we have the information to tell when buttons are being held
 * 
 * could do this either by using events to build up our controller state
 * or poll each button every frame
 * 
 * If we use events we still need to swap current button state to wasDown button state
 * every frame so might as well go with polling 
 */

#pragma once
#include <SDL.h>


class GameController
{
public:
	GameController();
	~GameController();
	int i = SDL_CONTROLLER_BUTTON_Y;
	 
	bool open(int index);
	void close();

	SDL_GameController* getSdlController() const{
		return gameController;
	};
	SDL_JoystickID getJoystickId() const{ 
		return SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController)); 
	}

	bool rumble();

	bool eventBelongsToController(SDL_Event& e);
	bool buttonEventBelongsToController(const SDL_Event& e);
	bool deviceEventBelongsToController(const SDL_Event& e);
	bool isConnected() { return connected; }

	void updateState();
	void swapState();
	void clearState();

	bool buttonDown(SDL_GameControllerButton button);
	bool buttonHeld(SDL_GameControllerButton button);
	bool buttonTriggered(SDL_GameControllerButton button);
	bool buttonReleased(SDL_GameControllerButton button);
	void printButtonState();
private:
	SDL_GameController* gameController;
	SDL_Haptic* controllerHaptic;
	//under the hood sdl game controllers are treated as joysticks. I think actually more than one joystick per controller
	//but don't worry about that
	//we use the joystick id as the unique identifier for a specific game controlelr
	SDL_JoystickID joystickId;

	//if isn't connected should probably (always?) close sdl_gameController 
	bool connected;
	bool haptic;

	bool buttonState[SDL_CONTROLLER_BUTTON_MAX];
	bool buttonWasDownState[SDL_CONTROLLER_BUTTON_MAX];

	//sdl axis have a value from -32768 to 32767
	//but triggers range from 0 to 32767
	int axisState[SDL_CONTROLLER_AXIS_MAX];

	//this is chosen when controller is added
	//we use it to recognise device events
	int deviceIndex;
};

