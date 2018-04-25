/*
 * Author: Ed Curran (b15021040)
 * Class: InputManager
 * Extends: InputMapper
 * Description: provides functionality to convert from sdl input 
 * into our raw input and add to the underlying InputManager
 * 
 * This is basically another mapping layer
 * to map from sdl inputs to engine raw inputs
 * we also normalise axis values from the range sdl gives us to -1f - 1f for
 * internal use
 */

#pragma once
#include "../Input/InputManager.h"
#include "GameController.h"
#include <SDL.h>

const int sdlAxisMaxValue = 32768;
const float internalAxisMaxValue = 1.0f;

class SdlInputManager :
	public InputManager
{
public:
	SdlInputManager();
	~SdlInputManager();

	bool addSdlKeyMapping(SDL_Keycode sdlKey, InputRaw::Buttons rawInputButton);
	bool addSdlGameControllerButtonMapping(SDL_GameControllerButton sdlButton, InputRaw::Buttons rawInputButton);
	bool addSdlGameControllerAxesMapping(SDL_GameControllerAxis sdlAxes, InputRaw::Axes rawInputAxes);

	bool addSdlKeyEvent(InputActors actor, SDL_Event &e);
	bool addSdlKeyUp(InputActors actor, SDL_Keycode keycode);
	bool addSdlKeyDown(InputActors actor, SDL_Keycode keycode, bool keyRepeat);

	
	void addSdlGameControllerState(InputActors actor, GameController& controller);
	void addSdlKeyboardState(InputActors actor);
	//i'll implement this when if i actually need to use a mouse,
	//it should be easy tbh
	void addSdlMouseState(InputActors actor);

	/*
	we know sdl game controller axis are in the range -32768 to 32768
	*/
	float normaliseSdlAxisValue(int value);
protected:
	bool mapSdlKeyToButton(SDL_Keycode keycode, InputRaw::Buttons& outButtonId);
	bool mapSdlControllerButtonToButton(SDL_GameControllerButton controllerButton, InputRaw::Buttons& outButtonId);
	bool mapSdlControllerAxesToAxes(SDL_GameControllerAxis controllerAxes, InputRaw::Axes& outAxesId);


	std::map<SDL_Keycode, InputRaw::Buttons>::iterator i;

	//mapping between sdl keys and engine raw input buttons
	std::map<SDL_Keycode, InputRaw::Buttons> sdlKeyMap;

	//mapping between sdl game controller buttons to raw input buttons
	std::map<SDL_GameControllerButton, InputRaw::Buttons> sdlGameControllerButtonMap;

	//mapping between sdl game controller axes to raw input axes
	std::map<SDL_GameControllerAxis, InputRaw::Axes> sdlGameControllerAxesMap;
};

