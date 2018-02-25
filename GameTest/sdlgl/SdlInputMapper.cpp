#include "SdlInputMapper.h"
#include <functional>


SdlInputMapper::SdlInputMapper()
{
}


SdlInputMapper::~SdlInputMapper()
{
}

bool SdlInputMapper::addSdlKeyMapping(SDL_Keycode sdlKey, InputRaw::Buttons rawInputButton){
	return sdlKeyMap.insert(std::make_pair(sdlKey, rawInputButton)).second == true;
}

bool SdlInputMapper::addSdlKeyEvent(SDL_Event& e){
	if(e.type == SDL_KEYDOWN){
		return addSdlKeyDown(e.key.keysym.sym, e.key.repeat);
		
	} else if (e.type == SDL_KEYUP){
		return addSdlKeyUp(e.key.keysym.sym);
	}
	return false;
}

bool SdlInputMapper::addSdlKeyUp(SDL_Keycode keycode){
	InputRaw::Buttons buttonId;
	if(mapSdlKeyToButton(keycode, buttonId)){
		addButtonUp(buttonId);
		return true;
	}
	return false;
	
}

bool SdlInputMapper::addSdlKeyDown(SDL_Keycode keycode, bool keyRepeat){
	InputRaw::Buttons buttonId;
	if (mapSdlKeyToButton(keycode, buttonId)) {
		addButtonDown(buttonId, keyRepeat);
		return true;
	}
	return false;
}

bool SdlInputMapper::mapSdlKeyToButton(SDL_Keycode keycode, InputRaw::Buttons& outButtonId){
	i = sdlKeyMap.find(keycode);

	if (i == sdlKeyMap.end()) {	/* Not found */
		return false;
	} else { //found
			 //return the mapped value
		outButtonId = i->second;
		return true;
	}
}


