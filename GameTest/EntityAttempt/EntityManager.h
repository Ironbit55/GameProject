#pragma once
#define NOMINMAX

#include "EntityContainer.h"
#include "WallEntity.h"
#include "EntityBall.h"
#include "DebrisEntity.h"
#include "../GameTest/cpptoml.h"

enum EntityType
{
	ENTITY_INVALID = -1,
	ENTITY_WALL,
	ENTITY_BALL,
	ENTITY_PLAYER,
	ENTITY_DEBRIS,
	ENTITY_MAX,
	
};

struct EntityDef {
	std::string name;
	EntityType entityType;
	std::string textureName;
	Vector2 position;
	float rotation;
	InputActors inputActor;
	int number;


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
	void createEntity(EntityDef& entityDef);

	EntityDef entityDefFromTOML(cpptoml::table& entityDefTable);

	void loadConfig(const std::string& configFile);

	EntityType asEntityType(int entityTypeId);
	EntityType asEntityType(std::string);


protected:
	TransformManager& transformManager;
	ContentManager& contentManager;
	std::map<std::string, EntityType> entityTypeMap;

};

