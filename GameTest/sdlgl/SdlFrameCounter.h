#pragma once
#include "SdlDeltaTimer.h"
#include <string>

class SdlFrameCounter
{
public:
	SdlFrameCounter();
	~SdlFrameCounter();

	void addFrame() { countedFrames++; }
	void drawFps(SDL_Window* w){
		std::string s = std::to_string(getAvgFps());
		SDL_SetWindowTitle(w, s.c_str());
		
	}
	void update(SDL_Window* w){
		addFrame();
		if (timer.getDeltaTimeMS() > 1000.0f) {
			avgFps = countedFrames;

			drawFps(w);
			timer.updateTime();
			countedFrames = 0;
		}
	}
	int getAvgFps(){
		return avgFps;
	}


private:
	int countedFrames;
	int avgFps;
	SdlDeltaTimer timer;
};

