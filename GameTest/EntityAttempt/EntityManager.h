#pragma once
#include "EntityContainer.h"
#include "WallEntity.h"
#include "EntityBall.h"
#include "DebrisEntity.h"

enum EntityType
{
	ENTITY_WALL,
	ENTITY_BALL,
	ENTITY_PLAYER,
	ENTITY_DEBRIS,
	ENTITY_MAX,
	
};

class EntityManager : public EntityContainer
{
public:
	EntityManager(TransformManager& tm, ContentManager& cm) : transformManager(tm), contentManager(cm) {};
	~EntityManager();;

	void update(TransformManager& transformManager) {
		deleteEntities(transformManager);
		EntityContainer::update();
	}

	EntityInterface* createWall(Vector2 position, float rotation = 0.0f);
	EntityInterface* createPlayer(InputActors inputActor, Vector2 position, float rotation = 0.0f);
	EntityBall* createBall(Vector2 position);
	EntityBall* createProjectile(Vector2 position, Vector2 launchDirection);
	
	DebrisEntity* createDebris(Vector2 position, Vector4 colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	void createWall(WallEntity& wallEntity) {};
	//void createPlayer(Vector2 position);
	//void createBall(Vector2 position);

	void createEntity(EntityType type, Vector2 position, float rotation = 0.0f) {};


protected:
	TransformManager& transformManager;
	ContentManager& contentManager;

};

