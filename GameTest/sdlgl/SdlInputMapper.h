/*
 * DEPRECATED DON'T USE THIS
 * go through SdlInputManager instead
 */
#pragma once
#include "../Input/InputMapper.h"
#include <SDL.h>

class GameController;

class SdlInputMapper :
	public InputMapper
{
public:
	SdlInputMapper();
	~SdlInputMapper();

	bool addSdlKeyMapping(SDL_Keycode sdlKey, InputRaw::Buttons rawInputButton);

	bool addSdlKeyEvent(SDL_Event &e);
	bool addSdlKeyUp(SDL_Keycode keycode);
	bool addSdlKeyDown(SDL_Keycode keycode, bool keyRepeat);

	bool addSdlGameControllerButtonDownEvent(SDL_Event &e);
	bool addSdlGameControllerAxisEvent(SDL_Event &e);

	bool addSdlGameControllerState(GameController& controller, int controllerIndex);

protected:
	bool mapSdlKeyToButton(SDL_Keycode keycode, InputRaw::Buttons& outButtonId);
	bool mapSdlControllerButtonToButton(SDL_GameControllerButton controllerButton, InputRaw::Buttons& outButtonId);
	bool mapSdlControllerAxesToAxes(SDL_GameControllerAxis controllerAxes, InputRaw::Axes& outAxesId);
	
	
	std::map<SDL_Keycode, InputRaw::Buttons>::iterator i;
	std::map<SDL_Keycode, InputRaw::Buttons> sdlKeyMap;
	std::map<SDL_GameControllerButton, InputRaw::Buttons> gameControllerButtonMap;
	std::map<SDL_GameControllerAxis, InputRaw::Axes> gameControllerAxesMap;

};

