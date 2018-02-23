#include "SdlWindow.h"


bool SdlWindow::init(){
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	if (fullScreen) {
		flags = flags | (SDL_WINDOW_FULLSCREEN);
	}
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
	if (window == NULL){
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

bool SdlWindow::createRenderContext(){
	if(!window){
		printf("Could not create render context! Window is not initialised: \n");
		return false;
	}

	// Set our OpenGL version.
	// we want the core context: SDL_GL_CONTEXT_CORE so deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//using version 3.2, modern opengl so we have shader etc
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// this is actuially on by default
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	mainContext = SDL_GL_CreateContext(window);

	if (mainContext == NULL){
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	//could initialise glew here?

	//I guess everything is good if we make it here
	return true;
}


