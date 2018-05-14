/*
* Class: FeedbackEngine
* Author: Ed Curran (b5021040)
* Description: The Real Deal, The Main Man this is the heart of the engine
  it holds all the subsystems, gets hooked into entrypoint to startup
  then updates subsystems, renders, handles events etc
  provides hooks for World instance to execute on top of the engine
*/

#pragma once
#include <SDL.h>
#include "sdlgl/SdlWindow.h"
#include "sdlgl/SdlInputManager.h"
#include "sdlgl/SdlFrameCounter.h"
#include "IrohRenderer/SpriteRenderer.h"
#include "TransformManager.h"
#include "AudioManager.h"
#include "ContentManager.h"
#include "sdlgl/SdlInput.h"
#include "World.h"

class FeedbackEngine
{
public:
	FeedbackEngine(int screenWidth, int screenHeight, int maxNumTextures);
	~FeedbackEngine();

	void initialise();

	World& attachWorld(World& world) {};

	//get a world object
	//this can be modified and will be used
	//on engine start
	World* createWorld();

	void update();

	void handleSdlEvent(SDL_Event e);

	void start();

	void end();

protected:
	SdlWindow window;
	SdlInputManager inputManager;
	SdlInput sdlInput;
	SpriteRenderer renderer;
	RenderSystem renderSystem;
	PhysicsSystem physicsSystem;
	TransformManager transformManager;
	AudioManager audioManager;
	ContentManager contentManager;

	void draw();

	void handleInput();

	void loadContent();

private:
	SdlDeltaTimer timer = SdlDeltaTimer();
	SdlFrameCounter frameCounter;

	bool initialiseSdl();

	bool initInput();

	bool loadInputMappings();

	float deltaTimeMs;

	//basically this is a data container, the engine takes it and runs it
	//and thats the game... hopefully
	World* world;
};

