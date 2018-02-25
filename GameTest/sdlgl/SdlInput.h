#pragma once
#include <SDL.h>
#include "SdlInputMapper.h"

class SdlInput
{
public:
	SdlInput(SdlInputMapper& inputMapper);
	~SdlInput();

	void handleEvent(SDL_Event &e);
private:
	SdlInputMapper& inputMapper;
};

