#include "SdlFrameCounter.h"



SdlFrameCounter::SdlFrameCounter(){
	countedFrames = 0;
	timer = SdlDeltaTimer();
}


SdlFrameCounter::~SdlFrameCounter()
{
}
