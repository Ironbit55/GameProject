#include "SdlInput.h"



SdlInput::SdlInput(SdlInputMapper& inputMapper) : inputMapper(inputMapper){
	inputMapper.addSdlKeyMapping(SDLK_w, InputRaw::Buttons::BUTTON_KEY_W);
	inputMapper.addSdlKeyMapping(SDLK_a, InputRaw::Buttons::BUTTON_KEY_A);
	inputMapper.addSdlKeyMapping(SDLK_s, InputRaw::Buttons::BUTTON_KEY_S);
	inputMapper.addSdlKeyMapping(SDLK_d, InputRaw::Buttons::BUTTON_KEY_D);
}


SdlInput::~SdlInput()
{
}

void SdlInput::handleEvent(SDL_Event& e){
	if (inputMapper.addSdlKeyEvent(e)) { return; }
	//handle controller events
}
