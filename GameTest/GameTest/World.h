#pragma once
#include "ContentManager.h"
#include "TransformManager.h"
#include "InputReceiver.h"

class World : InputReceiver
{
public:
	World(TransformManager& transformManager, Camera& camera) : transformManager(transformManager), camera(camera) {};
	~World();

	void initialise();;

	void loadLevel() {};

	void loadContent(ContentManager contentManager) {};

	void handleInput(InputActors inputActor, MappedInput* mappedInput) override;

	void update(Camera& camera) {};
protected:
	void controlCameraInput(MappedInput* mappedInput);
private:
	int test = 0;
	TransformManager& transformManager;
	Camera& camera;

};

