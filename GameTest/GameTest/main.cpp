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
#include "SpriteRenderer.h"

#pragma comment(lib, "Input.lib")
#pragma comment(lib, "sdlgl.lib")
#pragma comment(lib, "nclgl.lib")

////Screen dimension constants
const int SCREEN_WIDTH = 1800;
const int SCREEN_HEIGHT = 1000;
bool quit = false;

int frames = 0;
int avgFrames = 0;
float frameRateTimer = 0.0f;

//Event handler
SDL_Event e;

bool initSDL() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}



bool initInput(SdlInputManager& inputManager) {
	InputContext context = InputContext(true);

	
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_W, InputCooked::States::STATE_MOVE_UP);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_W, InputCooked::Actions::ACTION_JUMP);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_A, InputCooked::States::STATE_MOVE_LEFT);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_S, InputCooked::States::STATE_MOVE_DOWN);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_D, InputCooked::States::STATE_MOVE_RIGHT);

	
	inputManager.addInputContext(InputActors::INPUT_ACTOR_PLAYER1, "keyboard", context);
	inputManager.activateActor(InputActors::INPUT_ACTOR_PLAYER1);

	InputContext controllerInputContext = InputContext(true);

	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_A, InputCooked::States::STATE_CONTROLLER_A);
	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_X, InputCooked::States::STATE_CONTROLLER_X);
	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_X, InputCooked::Actions::ACTION_CONTROLLER_X);
	controllerInputContext.addMapping(InputRaw::Axes::AXIS_CONTROLLER_AXIS_LEFTX, InputCooked::Ranges::RANGE_CONTROLLER_LEFT_X);
	controllerInputContext.addMapping(InputRaw::Axes::AXIS_CONTROLLER_AXIS_LEFTY, InputCooked::Ranges::RANGE_CONTROLLER_LEFT_Y);

	inputManager.addInputContext(InputActors::INPUT_ACTOR_PLAYER1, "controller", controllerInputContext);
	inputManager.activateActor(InputActors::INPUT_ACTOR_PLAYER1);

	inputManager.addInputContext(InputActors::INPUT_ACTOR_PLAYER2, "controller", controllerInputContext);
	inputManager.activateActor(InputActors::INPUT_ACTOR_PLAYER2);
	
	return true;
}

/*
* sdl has a custom main we need to use as entry point
*/
int main(int argc, char* args[]) {
	initSDL();
	
	SdlWindow w = SdlWindow("game test", SCREEN_WIDTH, SCREEN_HEIGHT, false);
	w.init();
	

	SdlInputManager inputManager;
	
	initInput(inputManager);
	SdlInput sdlInput = SdlInput(inputManager);


	/*Renderer r(w);
	r.Init();*/

	/*Mesh*	m = Mesh::LoadMeshFile("cube.asciimesh");
	Shader* s = new Shader("basicvert.glsl", "basicFrag.glsl");

	if (s->UsingDefaultShader()) {
		cout << "Warning: Using default shader! Your shader probably hasn't worked..." << endl;
		cout << "Press any key to continue." << endl;
		std::cin.get();
	}



	RenderObject o(m, s);
	o.SetModelMatrix(Matrix4::Translation(Vector3(0, 0, -10)) * Matrix4::Scale(Vector3(1, 1, 1)) * Matrix4::Rotation(45, Vector3(1, 0, 0)));
	o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Scale(Vector3(1, 1, 1)));

	r.AddRenderObject(o);*/

	SpriteRenderer spriteRenderer(w);
	spriteRenderer.Init();
	if (spriteRenderer.GetCurrentShader()->UsingDefaultShader()) {
		cout << "Warning: Using default shader! Your shader probably hasn't worked..." << endl;
		cout << "Press any key to continue." << endl;
		std::cin.get();
	}

	Camera* camera = spriteRenderer.getCamera();

	// While application is running
	
	GameTimer timer = GameTimer();
	frameRateTimer = timer.GetMS();
	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;

			}

			sdlInput.handleEvent(e);
			w.handleEvent(e);
		}

		float msec = timer.GetTimedMS();
	
		frames++;

		if (timer.GetMS() - frameRateTimer > 1000.0f) {
			std::string s = std::to_string(frames);
			SDL_SetWindowTitle(w.getWindow(), s.c_str());

			frameRateTimer = timer.GetMS();
			frames = 0;
		}


		spriteRenderer.RenderScene();

		/*if(true){
			continue;
		}*/

		sdlInput.update();
		spriteRenderer.UpdateScene(msec);

		GameControllerContainer& controllerContainer = sdlInput.getControllerContainer();

		if(controllerContainer.getController(0).buttonTriggered(SDL_CONTROLLER_BUTTON_A)){
			cout << "A button triggered! game controller 1: joystick id = " + std::to_string(controllerContainer.getController(0).getJoystickId()) + "\n";
			controllerContainer.getController(0).rumble();
		}
		if (controllerContainer.getController(0).buttonDown(SDL_CONTROLLER_BUTTON_Y)) {
			cout << "Y button down! game controller 1: joystick id = " + std::to_string(controllerContainer.getController(0).getJoystickId()) + "\n";
		}
		if (controllerContainer.getController(1).buttonTriggered(SDL_CONTROLLER_BUTTON_X)) {
			cout << "X button triggered! game controller 2: joystick id = " + std::to_string(controllerContainer.getController(1).getJoystickId()) + "\n";
			controllerContainer.getController(1).rumble();
		}
		
		inputManager.performMapping();

		//o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Rotation(0.1f * msec, Vector3(0, 0, 1)));

		MappedInput& player1MappedInput = inputManager.getMappedInput(InputActors::INPUT_ACTOR_PLAYER1);
		if (player1MappedInput.getAction(InputCooked::Actions::ACTION_JUMP)) {
			cout << "Action Jump";
			
		}

		if(player1MappedInput.getState(InputCooked::States::STATE_MOVE_UP)){
			cout << "Move Up";
			camera->SetPosition(camera->GetPosition() + (Vector3(0.0f, -1.0f, 0.0f) * 10));
		}
		if (player1MappedInput.getState(InputCooked::States::STATE_MOVE_DOWN)) {
			cout << "Move Down";
			camera->SetPosition(camera->GetPosition() + Vector3(0.0f, 1.0f, 0.0f) * 10);
		}
		if (player1MappedInput.getState(InputCooked::States::STATE_MOVE_LEFT)) {
			cout << "Move Left";
			camera->SetPosition(camera->GetPosition() + Vector3(-1.0f, 0.0f, 0.0f) * 10);
		}
		if (player1MappedInput.getState(InputCooked::States::STATE_MOVE_RIGHT)) {
			cout << "Move Right";
			camera->SetPosition(camera->GetPosition() + Vector3(1.0f, 0.0f, 0.0f) * 10);
		}



		//controllers test

		//player 1
		if (player1MappedInput.getState(InputCooked::States::STATE_CONTROLLER_A)) {
			cout << "Player 1: Controller State A" << endl;
		}

		if (player1MappedInput.getState(InputCooked::States::STATE_CONTROLLER_X)) {
			cout << "Player 1: Controller State X" << endl;
		}

		if (player1MappedInput.getAction(InputCooked::Actions::ACTION_CONTROLLER_X)) {
			cout << "Player 1: Controller Action X" << endl;
		}

		float xRange = -1.0f;
		if (player1MappedInput.getRange(InputCooked::Ranges::RANGE_CONTROLLER_LEFT_X, xRange)) {
			//cout << "Player 1: Controller Range LEFTX = " << xRange << endl;
		}

		//float yRange = -1.0f;
		//if (player1MappedInput.getRange(InputCooked::Ranges::RANGE_CONTROLLER_LEFT_Y, yRange)) {
		//	cout << "Player 1: Controller Range LEFTY = " << yRange << endl;
		//}

		
		//player 2
		MappedInput& player2MappedInput = inputManager.getMappedInput(InputActors::INPUT_ACTOR_PLAYER2);

		if (player2MappedInput.getState(InputCooked::States::STATE_CONTROLLER_A)) {
			cout << "Player 2: Controller State A" << endl;
		}

		if (player2MappedInput.getState(InputCooked::States::STATE_CONTROLLER_X)) {
			cout << "Player 2: Controller State X" << endl;
		}

		if (player2MappedInput.getAction(InputCooked::Actions::ACTION_CONTROLLER_X)) {
			cout << "Player 2: Controller Action X" << endl;
		}

		xRange = -1.0f;
		if (player2MappedInput.getRange(InputCooked::Ranges::RANGE_CONTROLLER_LEFT_X, xRange)) {
			//cout << "Player 2: Controller Range LEFTX = " << xRange << endl;
		}

		//yRange = -1.0f;
		//if (player2MappedInput.getRange(InputCooked::Ranges::RANGE_CONTROLLER_LEFT_Y, yRange)) {
		//	cout << "Player 2: Controller Range LEFTY = " << yRange << endl;
		//}

	
		
		
		//r.UpdateScene(msec);
		//r.ClearBuffers();
		//r.RenderScene();
		//r.SwapBuffers();
		
		inputManager.clearMappedInput();
	}


	//delete m;
	//delete s;

	return 0;
}

//
//
//int main(int argc, char* args[]) {
//	initSDL();
//	SdlWindow sdlWindow = SdlWindow("test", 540, 540);
//	sdlWindow.init();
//	sdlWindow.createRenderContext();
//	// Set color
//	glClearColor(1.0, 0.0, 0.5, 1.0);
//
//	// Clear back buffer
//	glClear(GL_COLOR_BUFFER_BIT);
//	// Swap back and front buffer
//	SDL_GL_SwapWindow(sdlWindow.getWindow());
//	// While application is running
//	while (!quit)
//	{
//		//Handle events on queue
//		while (SDL_PollEvent(&e) != 0)
//		{
//			//User requests quit
//			if (e.type == SDL_QUIT)
//			{
//				quit = true;
//			}
//		}
//
//	}
//	return 0;
//}