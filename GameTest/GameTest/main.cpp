//this needs to be defined on windows otherwise box2d includes
//windows.h for some reason and max and min macro break cpptoml
//okay this isn't working for some reason, i've underfined min and max in cpptoml.h
//instead for now although this isn't ideal
#define NOMINMAX

#include <SDL.h>
#include "FeedbackEngine.h"

#pragma comment (lib, "EntityAttempt.lib")
#pragma comment(lib, "IrohRenderer.lib")
#pragma comment(lib, "Input.lib")
#pragma comment(lib, "sdlgl.lib")
#pragma comment(lib, "nclgl.lib")
#pragma comment(lib, "Box2D.lib")

//Screen dimension constants
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 1000;
bool quit = false;


//Event handler
SDL_Event e;


/*
* sdl has a custom main we need to use as entry point
*/
int main(int argc, char* args[]) {

	FeedbackEngine engine(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
	engine.initialise();

	//Entity testEntity;

	World* world = engine.createWorld();
	engine.start();

	// While application is running
	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;

			}

			engine.handleSdlEvent(e);
		}

		engine.update();

		
	}

	engine.end();
	return 0;
}

