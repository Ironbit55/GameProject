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
	ENTITY_GOAL,
	ENTITY_BALL_SPAWNER,
	ENTITY_PROJECTILE_SPAWNER,
	ENTITY_MAX,
	
};



struct EntityProperties {
	std::string textureName;
	Vector2 position;
	float rotation;
	InputActors inputActor;
	int number;
	
};

struct CreateEntityMsgData{
	EntityDef entityDef;
};

class EntityManager : public EntityContainer, MessageReceiver
{
public:
	EntityManager(TransformManager& tm, ContentManager& cm) : transformManager(tm), contentManager(cm){
		addListener(MESSAGE_CREATE_ENTITY);
	};
	~EntityManager();;

	void update(float msec, TransformManager& transformManager) {
		deleteEntities(transformManager);
		EntityContainer::update(msec);
	}

	EntityInterface* createWall(EntityDef entityDef);
	EntityInterface* createPlayer(InputActors inputActor, Vector2 position, float rotation = 0.0f);
	EntityInterface* createGoal(EntityDef);
	EntityInterface* createGameReferee(EntityDef);
	EntityInterface* createBallSpawner(EntityDef entityDef);
	EntityBall* createBall(EntityDef);
	EntityBall* createProjectile(Vector2 position, Vector2 launchDirection, float magnitude, InputActors inputActor);
	
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

	void onReceiveMessage(Message& msg) override;

};

