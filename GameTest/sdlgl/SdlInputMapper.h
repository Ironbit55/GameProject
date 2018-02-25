#pragma once
#include "../Input/InputMapper.h"
#include <SDL.h>

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

protected:
	bool mapSdlKeyToButton(SDL_Keycode keycode, InputRaw::Buttons& outButtonId);
	
	
	std::map<SDL_Keycode, InputRaw::Buttons>::iterator i;
	std::map<SDL_Keycode, InputRaw::Buttons> sdlKeyMap;

};

