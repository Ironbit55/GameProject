#include "SDLRenderer.h"



SDLRenderer::SDLRenderer(SdlWindow& window) : OGLRenderer(), window(window){}


SDLRenderer::~SDLRenderer(){
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);
}
/*
 * as we are using SDL this actually should be cross platform... hopefully
 */
void SDLRenderer::SwapBuffers(){
	DrawDebug();
	SDL_GL_SwapWindow(window.getWindow());
}

bool SDLRenderer::Init(){
	if (!window.getWindow()) {
		printf("Could not create render context! Window is not initialised: \n");
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

	mainContext = SDL_GL_CreateContext(window.getWindow());

	if (mainContext == NULL) {
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(0);

	window.registerResizeCallback(BasicResizeFunc); //Tell our window about the new renderer! (Which will in turn resize the renderer window to fit...)	

	//could refactor initialising GLEW into core OGLRenderer
	//in fact everything from here down could be pushed into OGLRenderer as it stands

	glewExperimental = GL_TRUE;	//This forces GLEW to give us function pointers for everything (gets around GLEW using 'old fashioned' methods
								//for determining whether a OGL context supports a particular function or not

	if (glewInit() != GLEW_OK) {	//Try to initialise GLEW
		std::cout << "OGLRenderer::OGLRenderer(): Cannot initialise GLEW!" << std::endl;	//It's all gone wrong!
		return false;
	}


	glClearColor(0.392f, 0.584f, 0.96f, 1.0f);

	InitDebugRenderer();

	initialised = true;

	//I guess everything is good if we make it here
	return true;
}
