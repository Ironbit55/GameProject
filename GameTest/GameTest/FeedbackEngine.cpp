#include "FeedbackEngine.h"



FeedbackEngine::FeedbackEngine(int screenWidth, int screenHeight, int maxNumTextures = 24) : 
		window("game test", screenWidth, screenHeight, false),
		renderer(window), renderSystem(renderer), physicsSystem(), transformManager(renderSystem, physicsSystem), contentManager(maxNumTextures, TEXTUREDIR),
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
	initInput();
	sdlInput.loadMappings();
	renderer.Init();
	renderer.enableBatching();
}


World* FeedbackEngine::createWorld(){
	world = new World(transformManager, contentManager, *renderer.getCamera());
	return world;
}

void FeedbackEngine::update(){
	deltaTimeMs = timer.getDeltaTimeMS();
	timer.updateTime();
	frameCounter.update(window.getWindow());

	sdlInput.update();
	inputManager.performMapping();

	world->update(*(renderer.getCamera()));
	physicsSystem.update(deltaTimeMs);
	renderSystem.update(deltaTimeMs, contentManager);
	
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
	world->loadContent(contentManager);
	world->initialise();
}

void FeedbackEngine::end(){
	//make sure everything shuts down
	delete world;
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

	//player1 keyboard context, used for admin stuff
	InputContext context = InputContext(true);


	context.addMapping(InputRaw::Buttons::BUTTON_KEY_W, InputCooked::States::STATE_CAMERA_MOVE_UP);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_M, InputCooked::Actions::ACTION_TOGGLE_MUSIC);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_R, InputCooked::Actions::ACTION_RESET);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_A, InputCooked::States::STATE_CAMERA_MOVE_LEFT);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_S, InputCooked::States::STATE_CAMERA_MOVE_DOWN);
	context.addMapping(InputRaw::Buttons::BUTTON_KEY_D, InputCooked::States::STATE_CAMERA_MOVE_RIGHT);


	inputManager.addInputContext(InputActors::INPUT_ACTOR_PLAYER1, "keyboard", context);
	inputManager.activateActor(InputActors::INPUT_ACTOR_PLAYER1);

	//game keyboard input, used by player 1
	InputContext keyboardGameContext = InputContext(true);

	keyboardGameContext.addMapping(InputRaw::Buttons::BUTTON_KEY_UP, InputCooked::States::STATE_PLAYER_MOVE_UP);
	keyboardGameContext.addMapping(InputRaw::Buttons::BUTTON_KEY_DOWN, InputCooked::States::STATE_PLAYER_MOVE_DOWN);
	keyboardGameContext.addMapping(InputRaw::Buttons::BUTTON_KEY_LEFT, InputCooked::States::STATE_PLAYER_MOVE_LEFT);
	keyboardGameContext.addMapping(InputRaw::Buttons::BUTTON_KEY_RIGHT, InputCooked::States::STATE_PLAYER_MOVE_RIGHT);
	keyboardGameContext.addMapping(InputRaw::Buttons::BUTTON_KEY_SPACE, InputCooked::Actions::ACTION_JUMP);

	inputManager.addInputContext(InputActors::INPUT_ACTOR_PLAYER1, "keyboard_game", keyboardGameContext);
	inputManager.activateActor(InputActors::INPUT_ACTOR_PLAYER1);

	//gamepad context, shared by all players
	
	InputContext controllerInputContext = InputContext(true);

	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_A, InputCooked::States::STATE_CONTROLLER_A);
	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_A, InputCooked::Actions::ACTION_JUMP);
	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_X, InputCooked::States::STATE_CONTROLLER_X);
	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_X, InputCooked::Actions::ACTION_CONTROLLER_X);

	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_DPAD_UP, InputCooked::States::STATE_PLAYER_MOVE_UP);
	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_DPAD_DOWN, InputCooked::States::STATE_PLAYER_MOVE_DOWN);
	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_DPAD_LEFT, InputCooked::States::STATE_PLAYER_MOVE_LEFT);
	controllerInputContext.addMapping(InputRaw::Buttons::BUTTON_CONTROLLER_BUTTON_DPAD_RIGHT, InputCooked::States::STATE_PLAYER_MOVE_RIGHT);

	controllerInputContext.addMapping(InputRaw::Axes::AXIS_CONTROLLER_AXIS_LEFTX, InputCooked::Ranges::RANGE_CONTROLLER_LEFT_X);
	controllerInputContext.addMapping(InputRaw::Axes::AXIS_CONTROLLER_AXIS_LEFTY, InputCooked::Ranges::RANGE_CONTROLLER_LEFT_Y);

	controllerInputContext.addDeadzone(0.1f);

	inputManager.addInputContext(InputActors::INPUT_ACTOR_PLAYER1, "controller", controllerInputContext);
	inputManager.activateActor(InputActors::INPUT_ACTOR_PLAYER1);

	inputManager.addInputContext(InputActors::INPUT_ACTOR_PLAYER2, "controller", controllerInputContext);
	inputManager.activateActor(InputActors::INPUT_ACTOR_PLAYER2);


	return true;

}
