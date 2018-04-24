#pragma once
#include "EntityContainer.h"
#include "WallEntity.h"

enum EntityType
{
	Wall,
	Ball,
	Player,
};

class EntityManager : public EntityContainer
{
public:
	EntityManager(TransformManager& tm, ContentManager& cm) : transformManager(tm), contentManager(cm) {};
	~EntityManager() {};

	void update(TransformManager& transformManager) {
		deleteEntities(transformManager);
		EntityContainer::update();
	}

	EntityInterface* createWall(Vector2 position, float rotation = 0.0f);;
	EntityInterface* createPlayer(InputActors inputActor, Vector2 position, float rotation = 0.0f);;
	EntityInterface* createBall(Vector2 position, float rotation = 0.0f) {};

	void createWall(WallEntity& wallEntity) {};
	//void createPlayer(Vector2 position);
	//void createBall(Vector2 position);

	void createEntity(EntityType type, Vector2 position, float rotation = 0.0f) {};


protected:
	TransformManager& transformManager;
	ContentManager& contentManager;

};

