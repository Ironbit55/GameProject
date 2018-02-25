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

#pragma comment(lib, "Input.lib")
#pragma comment(lib, "sdlgl.lib")
#pragma comment(lib, "nclgl.lib")

////Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
bool quit = false;


//Event handler
SDL_Event e;

bool initSDL() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

//void main(void) {
//	Window w = Window("game test", 800, 600);
//	Renderer r(w);
//
//	Mesh*	m	= Mesh::LoadMeshFile("cube.asciimesh");
//	Shader* s	= new Shader("basicvert.glsl", "basicFrag.glsl");
//
//	if (s->UsingDefaultShader()) {
//		cout << "Warning: Using default shader! Your shader probably hasn't worked..." << endl;
//		cout << "Press any key to continue." << endl;
//		std::cin.get();
//	}
//
//
//
//	RenderObject o(m,s);
//	o.SetModelMatrix(Matrix4::Translation(Vector3(0, 0, -10)) * Matrix4::Scale(Vector3(1, 1, 1)) * Matrix4::Rotation(45, Vector3(1, 0, 0)));
//	o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Scale(Vector3(1,1,1)));
//	r.AddRenderObject(o);
//
//
//
//	while(w.UpdateWindow()) {
//		float msec = w.GetTimer()->GetTimedMS();
//
//		o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Rotation(0.1f * msec,Vector3(0,0,1)));
//
//		r.UpdateScene(msec);
//		r.ClearBuffers();
//		r.RenderScene();
//		r.SwapBuffers();
//	}
//	delete m;
//	delete s;
//}

/*
 * sdl has a custom main we need to use as entry point 
 */
bool initInput(InputMapper& mapper) {
	InputContext context = InputContext(true);

	
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_W, InputCooked::States::STATE_MOVE_UP);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_W, InputCooked::Actions::ACTION_JUMP);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_A, InputCooked::States::STATE_MOVE_LEFT);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_S, InputCooked::States::STATE_MOVE_DOWN);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_D, InputCooked::States::STATE_MOVE_RIGHT);
	
	mapper.addInputContext("test", context);
	return true;
}
int main(int argc, char* args[]) {
	initSDL();
	GameTimer timer = GameTimer();
	SdlWindow w = SdlWindow("game test", 540, 540);
	w.init();
	Renderer r(w);
	r.Init();

	
	SdlInputMapper inputMapper;
	initInput(inputMapper);
	SdlInput sdlInput = SdlInput(inputMapper);

	GameController* gameController = new GameController(0);
	GameController* gameController2 = new GameController(1);

	Mesh*	m = Mesh::LoadMeshFile("cube.asciimesh");
	Shader* s = new Shader("basicvert.glsl", "basicFrag.glsl");

	if (s->UsingDefaultShader()) {
		cout << "Warning: Using default shader! Your shader probably hasn't worked..." << endl;
		cout << "Press any key to continue." << endl;
		std::cin.get();
	}



	RenderObject o(m, s);
	o.SetModelMatrix(Matrix4::Translation(Vector3(0, 0, -10)) * Matrix4::Scale(Vector3(1, 1, 1)) * Matrix4::Rotation(45, Vector3(1, 0, 0)));
	o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Scale(Vector3(1, 1, 1)));
	r.AddRenderObject(o);

	// While application is running
	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;

			}

			if(e.type == SDL_CONTROLLERBUTTONDOWN){
				if(gameController->buttonEventBelongsToController(e)){
					cout << "button down on controller 0\n";
				}

				if (gameController2->buttonEventBelongsToController(e)) {
					cout << "button down on controller 1\n";
				}
			}

			if (e.type == SDL_CONTROLLERDEVICEADDED) {
				cout << "controller added with which id:" + std::to_string(e.cdevice.which) + "\n";
				
			}

			if (e.type == SDL_KEYDOWN) {
				if(e.key.keysym.sym == SDLK_c){
					cout << "adding controller...";
					delete gameController;
					gameController = new GameController(0);
				}
			}

			sdlInput.handleEvent(e);
			w.handleEvent(e);
		}

		inputMapper.dispatch();

		float msec = timer.GetTimedMS();

		o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Rotation(0.1f * msec, Vector3(0, 0, 1)));

		if (inputMapper.getMappedInput().getAction(InputCooked::Actions::ACTION_JUMP)) {
			cout << "Action Jump";
		}

		if(inputMapper.getMappedInput().getState(InputCooked::States::STATE_MOVE_UP)){
			cout << "Move Up";
		}
		if (inputMapper.getMappedInput().getState(InputCooked::States::STATE_MOVE_DOWN)) {
			cout << "Move Down";
		}
		if (inputMapper.getMappedInput().getState(InputCooked::States::STATE_MOVE_LEFT)) {
			cout << "Move Left";
		}
		if (inputMapper.getMappedInput().getState(InputCooked::States::STATE_MOVE_RIGHT)) {
			cout << "Move Right";
		}
		
		r.UpdateScene(msec);
		r.ClearBuffers();
		r.RenderScene();
		r.SwapBuffers();
		
		inputMapper.clearMappedInput();
	}

	delete gameController;
	delete gameController2;
	delete m;
	delete s;

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