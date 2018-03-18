#pragma once
#include <SDL_hints.h>
#include <SDL.h>

class SdlDeltaTimer
{
public:
	SdlDeltaTimer();
	~SdlDeltaTimer();

	float getTime();
	Uint32 getTimeMS();
	float getDeltaTimeMS();

	void updateTime();
private:
	//start time in ms
	Uint32 startTime;

	//time when updateTime was last called
	Uint32 lastTime;

	//tracks time passed between updateTime() calls in milliseconds
	float deltaTime;
};

