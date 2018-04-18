#include "FeedbackEngine.h"



FeedbackEngine::FeedbackEngine(int screenWidth, int screenHeight, int maxNumTextures = 24) : 
		window("game test", screenWidth, screenHeight, false),
		renderer(window), renderSystem(renderer), physicsSystem(), transformManager(renderSystem, physicsSystem), contentManager(maxNumTextures),
		sdlInput(inputManager)
{
	deltaTimeMs = 0;
}

FeedbackEngine::~FeedbackEngine(){
}

void FeedbackEngine::initialise(){
	initialiseSdl();
	window.init();
	audioManager.init();
	sdlInput.loadMappings();
}


World* FeedbackEngine::createWorld(){
	world = new World(transformManager);
	return world;
}

void FeedbackEngine::update(){
	deltaTimeMs = timer.getDeltaTimeMS();
	timer.updateTime();
	frameCounter.update(window.getWindow());
	inputManager.performMapping();

	physicsSystem.update(deltaTimeMs);
	renderSystem.update(deltaTimeMs);
	renderSystem.renderScene();

	MessagingService::instance().update(deltaTimeMs);
	MessagingService::instance().dispatchMessages();

	inputManager.swapRawInput();
	inputManager.clearMappedInput();

}

void FeedbackEngine::handleSdlEvent(SDL_Event e) {
	sdlInput.handleEvent(e);
}

void FeedbackEngine::start(){

}

void FeedbackEngine::end(){
	//make sure everything shuts
}

bool FeedbackEngine::initialiseSdl(){
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	return success;
}

bool FeedbackEngine::initInput(){

	//should read mapppings from file...

	//player1 keyboard context
	InputContext context = InputContext(true);


	context.addMapping(InputRaw::Buttons::BUTTON_KEY_W, InputCooked::States::STATE_MOVE_UP);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_W, InputCooked::Actions::ACTION_JUMP);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_A, InputCooked::States::STATE_MOVE_LEFT);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_S, InputCooked::States::STATE_MOVE_DOWN);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_D, InputCooked::States::STATE_MOVE_RIGHT);


	inputManager.addInputContext(InputActors::INPUT_ACTOR_PLAYER1, "keyboard", context);
	inputManager.activateActor(InputActors::INPUT_ACTOR_PLAYER1);

	//gamepad context, shared by all players
	
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
