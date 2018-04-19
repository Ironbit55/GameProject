#pragma once
#include "ContentManager.h"
#include "TransformManager.h"
#include "InputReceiver.h"

class World : InputReceiver
{
public:
	World(TransformManager& transformManager, Camera& camera) : transformManager(transformManager), camera(camera) {};
	~World();

	

	void loadLevel() {};

	void loadContent(ContentManager& contentManager);

	void initialise();

	void handleInput(InputActors inputActor, MappedInput* mappedInput) override;

	void update(Camera& camera) {};
protected:
	void controlCameraInput(MappedInput* mappedInput);
private:
	int dragonTextureId;
	int wallTextureId;
	int raiderTextureId;

	int test = 0;
	TransformManager& transformManager;
	Camera& camera;

};

