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
#include "../EntityAttempt/Entity.h"
#include "../sdlgl/SdlDeltaTimer.h"
#include "../sdlgl/SdlFrameCounter.h"
#include "../sdlgl/SdlMixer.h"
#include "AudioManager.h"

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

	controllerInputContext.addDeadzone(0.1f);

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
	Entity testEntity;
	//inputManager.registerListener(InputActors::INPUT_ACTOR_PLAYER1, (testEntity.inputListener));

	SpriteRenderer spriteRenderer(w);
	spriteRenderer.Init();

	AudioManager audioManager;
	audioManager.init();

	Message msg;
	msg.messageType = MESSAGE_AUDIO_EFFECT;
	msg.timeUntillDispatch = 0;
	SoundEffectMsgData msgData;
	msgData.effect = EFFECT_JUMP;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(msgData);
	

	Camera* camera = spriteRenderer.getCamera();

	// While application is running
	
	SdlDeltaTimer timer = SdlDeltaTimer();
	SdlFrameCounter frameCounter;
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

		float msec = timer.getDeltaTimeMS();
		timer.updateTime();
		frameCounter.update(w.getWindow());


		spriteRenderer.RenderScene();


		sdlInput.update();
		spriteRenderer.UpdateScene(msec);

		GameControllerContainer& controllerContainer = sdlInput.getControllerContainer();
		//GameController* controllerPtr = nullptr;
		if (controllerContainer.controllerIsConnected(0)) {
			//GameController& controller = controllerContainer.getController(0);
			//if (controller.buttonTriggered(SDL_CONTROLLER_BUTTON_A)) {
			//	cout << "A button triggered! game controller 1: joystick id = " + std::to_string(controller.getJoystickId()) + "\n";
			//	controller.rumble();
			//}
			//if (controller.buttonDown(SDL_CONTROLLER_BUTTON_Y)) {
			//	cout << "Y button down! game controller 1: joystick id = " + std::to_string(controller.getJoystickId()) + "\n";
			//}

			
		}
		if (controllerContainer.controllerIsConnected(1)) {
			/*GameController& controller = controllerContainer.getController(1);
			if (controller.buttonTriggered(SDL_CONTROLLER_BUTTON_X)) {
				cout << "X button triggered! game controller 2: joystick id = " + std::to_string(controller.getJoystickId()) + "\n";
				controller.rumble();
			}*/
		}
		
		inputManager.performMapping();

		//o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Rotation(0.1f * msec, Vector3(0, 0, 1)));

		MappedInput& player1MappedInput = inputManager.getMappedInput(InputActors::INPUT_ACTOR_PLAYER1);
		if (player1MappedInput.getAction(InputCooked::Actions::ACTION_JUMP)) {
			audioManager.playSoundEffect(EFFECT_JUMP);
			cout << "Action Jump";
			
		}

		float speed = 400;
		if(player1MappedInput.getState(InputCooked::States::STATE_MOVE_UP)){
			cout << "Move Up";
			//audioManager.playSoundEffect(EFFECT_JUMP);
			//MessagingService::instance().pushMessage(msg);
			camera->SetPosition(camera->GetPosition() + Vector3(0.0f, -1.0f, 0.0f) * speed * (msec / 1000));
		}

		if (player1MappedInput.getState(InputCooked::States::STATE_MOVE_DOWN)) {
			cout << "Move Down";
			camera->SetPosition(camera->GetPosition() + Vector3(0.0f, 1.0f, 0.0f) * speed * (msec / 1000));
		}
		if (player1MappedInput.getState(InputCooked::States::STATE_MOVE_LEFT)) {
			cout << "Move Left";
			camera->SetPosition(camera->GetPosition() + Vector3(-1.0f, 0.0f, 0.0f) * speed * (msec / 1000));
		}
		if (player1MappedInput.getState(InputCooked::States::STATE_MOVE_RIGHT)) {
			cout << "Move Right";
			camera->SetPosition(camera->GetPosition() + Vector3(1.0f, 0.0f, 0.0f) * speed * (msec / 1000));
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
			GameControllerContainer& controllerContainer = sdlInput.getControllerContainer();
			//GameController* controllerPtr = nullptr;
			if (controllerContainer.controllerIsConnected(0)) {
				GameController& controller = controllerContainer.getController(0);
				controller.rumble();
			}
			cout << "Player 1: Controller Action X" << endl;
		}

		float xRange = -1.0f;
		if (player1MappedInput.getRange(InputCooked::Ranges::RANGE_CONTROLLER_LEFT_X, xRange)) {
			cout << "Player 1: Controller Range LEFTX = " << xRange << endl;
		}

		float yRange = -1.0f;
		if (player1MappedInput.getRange(InputCooked::Ranges::RANGE_CONTROLLER_LEFT_Y, yRange)) {
			cout << "Player 1: Controller Range LEFTY = " << yRange << endl;
		}

		
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

	
		MessagingService::instance().update(msec);
		MessagingService::instance().dispatchMessages();
		
		//r.UpdateScene(msec);
		//r.ClearBuffers();
		//r.RenderScene();
		//r.SwapBuffers();
		
		inputManager.swapRawInput();
		inputManager.clearMappedInput();
	}


	//delete m;
	//delete s;

	return 0;
}

