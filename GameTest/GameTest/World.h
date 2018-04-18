#pragma once
#include "ContentManager.h"
#include "TransformManager.h"

class World
{
public:
	World(TransformManager transformManager) : transformManager(transformManager) {};
	~World();

	void initialise() {};

	void loadLevel() {};

	void loadContent(ContentManager contentManager) {};

	void handleInput() {};

	void update() {};
private:
	int test = 0;
	TransformManager& transformManager;
};

