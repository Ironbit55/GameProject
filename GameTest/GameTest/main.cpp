#include <SDL.h>

#include "Renderer.h"
#include "RenderObject.h"
#include <stdio.h>
#include <iostream>
#include "../sdlgl/SdlWindow.h"
#include "../sdlgl/SDLRenderer.h"
#include "../Input/InputMapper.h"
#include "../sdlgl/SdlInputMapper.h"
#include "../sdlgl/SdlInput.h"
#include "../sdlgl/GameController.h"
#include "../sdlgl/GameControllerContainer.h"
#include "../Input/InputManager.h"
#include "../sdlgl/SdlInputManager.h"
#include "../IrohRenderer/SpriteRenderer.h"
#include "../EntityAttempt/EntityInterface.h"
#include "../sdlgl/SdlDeltaTimer.h"
#include "../sdlgl/SdlFrameCounter.h"
#include "../sdlgl/SdlMixer.h"
#include "AudioManager.h"
#include "IrohRenderer/RenderSystem.h"
#include "PhysicsSystem.h"
#include "TransformManager.h"
#include "FeedbackEngine.h"

#pragma comment (lib, "EntityAttempt.lib")
#pragma comment(lib, "IrohRenderer.lib")
#pragma comment(lib, "Input.lib")
#pragma comment(lib, "sdlgl.lib")
#pragma comment(lib, "nclgl.lib")
#pragma comment(lib, "Box2D.lib")
//#pragma comment(lib, "MemoryManagement.lib")

////Screen dimension constants
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

