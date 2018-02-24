#pragma once
#include "..\..\nclgl\OGLRenderer.h"
#include "SdlWindow.h"

class SDLRenderer :
	public OGLRenderer
{
public:
	SDLRenderer(SdlWindow& window);
	~SDLRenderer();

	bool Init() override;
	void SwapBuffers() override;

private:
	SdlWindow& window;

	// Our sdl opengl context handle
	SDL_GLContext mainContext;
};
