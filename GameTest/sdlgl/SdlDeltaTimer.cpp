#include "SdlDeltaTimer.h"



SdlDeltaTimer::SdlDeltaTimer(){
	startTime = SDL_GetTicks();
	lastTime = 0;
	deltaTime = 0.0f;
}


SdlDeltaTimer::~SdlDeltaTimer(){
}

float SdlDeltaTimer::getTime(){
	return (SDL_GetTicks() - startTime) / 1000.0f;
}

Uint32 SdlDeltaTimer::getTimeMS(){
	return SDL_GetTicks() - startTime;
}

float SdlDeltaTimer::getDeltaTimeMS(){
	return getTimeMS() - (float)lastTime;
}

void SdlDeltaTimer::updateTime(){
	deltaTime = (getTimeMS() - (float)lastTime);
	lastTime = getTimeMS();
}
