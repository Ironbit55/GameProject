#include "EntityManager.h"
#include "PlayerEntity.h"
#include "GoalEntity.h"
#include "GameRefereeEntity.h"
#include "BallSpawnerEntity.h"

EntityManager::~EntityManager() {
	destructAllEntities(transformManager);
}

EntityInterface* EntityManager::createWall(EntityDef entityDef) {
	
	EntityInterface* entity = addEntity(sizeof(WallEntity));
	if(entity == nullptr){
		return nullptr;
	}

	//alternativly construct local Wall instance
	//cast EntityInterface to WallEntity* and assign to &wall
	//this means we need to make sure to implement copy constructors correctly etc

	WallEntity* wallPtr = reinterpret_cast<WallEntity*>(entity);
	

	//we have the memory location so construct it using placement new

	//i think everything may be out of allignment so everything will probably break horribly at some point
	//the memory pool handles out allignement but only for the object type you initialse it with...
	new(wallPtr) WallEntity();
	wallPtr->initialise(contentManager, transformManager, entityDef);
	

	return entity;


}

EntityInterface* EntityManager::createPlayer(InputActors inputActor, Vector2 position, float rotation) {
	EntityInterface* entity =  addEntity(sizeof(PlayerEntity));
	if(entity == nullptr){
		return nullptr;
	}

	PlayerEntity* playerPtr = reinterpret_cast<PlayerEntity*>(entity);
	new(playerPtr) PlayerEntity();
	playerPtr->initialise(contentManager, transformManager, inputActor, position, rotation);

	//we have to reigster input once we know the final destination pointer of the entity
	//as the callback needs to be bound to the correct instance
	//this is awkward and this is easiest way to do it...
	//playerPtr->rebindCallback();
	//playerPtr->registerInput(inputActor);
	return entity;
}

EntityInterface* EntityManager::createGoal(EntityDef entityDef) {
	EntityInterface* entity = addEntity(sizeof(PlayerEntity));
	if (entity == nullptr) {
		return nullptr;
	}

	GoalEntity* goalPtr = reinterpret_cast<GoalEntity*>(entity);
	new(goalPtr) GoalEntity();
	goalPtr->initialise(contentManager, transformManager, entityDef);


	return entity;
}

EntityInterface* EntityManager::createGameReferee(EntityDef entityDef) {
	EntityInterface* entity = addEntity(sizeof(GameRefereeEntity));
	if (entity == nullptr) {
		return nullptr;
	}

	GameRefereeEntity* refereePtr = reinterpret_cast<GameRefereeEntity*>(entity);
	new(refereePtr) GameRefereeEntity();
	refereePtr->initialise(contentManager, transformManager, entityDef);


	return entity;
}

EntityInterface* EntityManager::createBallSpawner(EntityDef entityDef) {
	EntityInterface* entity = addEntity(sizeof(BallSpawnerEntity));
	if (entity == nullptr) {
		return nullptr;
	}

	BallSpawnerEntity* refereePtr = reinterpret_cast<BallSpawnerEntity*>(entity);
	new(refereePtr) BallSpawnerEntity();
	refereePtr->initialise(contentManager, transformManager, entityDef);


	return entity;
}

EntityBall* EntityManager::createBall(EntityDef entityDef) {
	EntityInterface* entity = addEntity(sizeof(PlayerEntity));
	if (entity == nullptr) {
		return nullptr;
	}

	EntityBall* ballPtr = reinterpret_cast<EntityBall*>(entity);
	new(ballPtr) EntityBall();
	ballPtr->initialise(contentManager, transformManager, entityDef);

	return ballPtr;

}

EntityBall* EntityManager::createProjectile(Vector2 position, Vector2 launchDirection, float magnitude, InputActors playerIndex) {
	EntityDef entityDef;
	entityDef.position = position;
	entityDef.inputActor = playerIndex;
	EntityBall* entity = createBall(entityDef);
	if (entity == nullptr) {
		return nullptr;
	}

	entity->launch(launchDirection, magnitude);
	return entity;

}

DebrisEntity* EntityManager::createDebris(Vector2 position, Vector4 colour) {
	EntityInterface* entity = addEntity(sizeof(DebrisEntity));
	if (entity == nullptr) {
		return nullptr;
	}

	DebrisEntity* debrisPtr = reinterpret_cast<DebrisEntity*>(entity);
	new(debrisPtr) DebrisEntity();
	debrisPtr->initialise(contentManager, transformManager, position, colour);

	return debrisPtr;

}

void EntityManager::createEntity(EntityDef& entityDef) {
	switch (entityDef.entityType){
	case ENTITY_WALL: {
		createWall(entityDef);
		break;
	}

	case ENTITY_PLAYER: {
		createPlayer(entityDef.inputActor, entityDef.position, entityDef.rotation);
		break;
	}

	case ENTITY_BALL: {
		createBall(entityDef);
		break;
	}
	case ENTITY_DEBRIS: {
		//this is just for fun
		for (int i = 0; i < entityDef.number; ++i) {
			Vector2 position = Vector2((-800.0f + (i % 120) * 8), (600.0f - ((i / 180)) * 8));
			createDebris(position, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		}
		break;
	}
	case ENTITY_GOAL: {
		createGoal(entityDef);
		break;
	}
	case ENTITY_BALL_SPAWNER: {
		createBallSpawner(entityDef);
		break;
	}
	default:
		break;
	}
}

EntityDef EntityManager::entityDefFromTOML(cpptoml::table& entityDefTable) {
	EntityDef entityDef;
	entityDef.name = entityDefTable.get_as<std::string>("name").value_or("");

	//attempt to use "type_name" to determine type
	entityDef.entityType = asEntityType(entityDefTable.get_as<std::string>("type").value_or(""));
	//if doesn't exist or not valid then look for "type_id"
	if (entityDef.entityType == ENTITY_INVALID) {
		entityDef.entityType = asEntityType(entityDefTable.get_as<int64_t>("type_id").value_or(-1));
	}
	////if that doesn't exist then entity type is invalid, which means the entity def itself it basically invalid

	entityDef.position.x = entityDefTable.get_as<double>("position_x").value_or(0);
	entityDef.position.y = entityDefTable.get_as<double>("position_y").value_or(0);
	entityDef.scale.x = entityDefTable.get_as<double>("scale_x").value_or(100.0f);
	entityDef.scale.y = entityDefTable.get_as<double>("scale_y").value_or(100.0f);
	entityDef.rotation = entityDefTable.get_as<double>("rotation").value_or(0);
	int inputActorId = entityDefTable.get_as<int>("player").value_or(-1);
	if (InputManager::isInputActor(inputActorId)) {
		entityDef.inputActor = InputManager::asInputActor(inputActorId);
		if (entityDef.inputActor == INPUT_ACTOR_INVALID) {
			printf("could not load entity with input actor, input actor with id: %i does not exist", inputActorId);
		}
		
	}else {
		//probably wearnt expected entity def to contain a valid actor anyway
		entityDef.inputActor = InputActors::INPUT_ACTOR_INVALID;
	}
	entityDef.number = entityDefTable.get_as<int>("number").value_or(0);
	entityDef.textureName = entityDefTable.get_as<std::string>("texture").value_or("");
	entityDef.spawn = entityDefTable.get_as<bool>("spawn").value_or(false);
	return entityDef;
}

void EntityManager::loadConfig(const std::string& levelFileName) {
	auto config = cpptoml::parse_file(levelFileName);

	auto entityTypeMappings = config->get_table_array("entity_type");
	for (const auto& table : *entityTypeMappings) {
		// *table is a cpptoml::table
		auto typeId = table->get_as<int>("id");
		auto typeName = table->get_as<std::string>("name");

		//check mapping entry contains values for id and name
		if(typeId && typeName){

			EntityType type = asEntityType(*typeId);
			if (type != ENTITY_INVALID && !(*typeName).empty()) {
				//only add valid mappings, aka between a valid entityType and a non empty string
				entityTypeMap.insert(std::pair<std::string, EntityType>(*typeName, type));
			}
		}
		
	}

}

EntityType EntityManager::asEntityType(int entityTypeId) {
	if(entityTypeId < 0 || entityTypeId > ENTITY_MAX){
		return ENTITY_INVALID;
	}

	return static_cast<EntityType>(entityTypeId);
}

EntityType EntityManager::asEntityType(std::string typeName) {
	auto it = entityTypeMap.find(typeName);
	if(it == entityTypeMap.end()){
		//no mapping
		return ENTITY_INVALID;
	}

	return asEntityType(it->second);
}

void EntityManager::onReceiveMessage(Message& msg) {
	switch(msg.messageType)
	{
		case MESSAGE_CREATE_ENTITY: {
			CreateEntityMsgData* data = static_cast<CreateEntityMsgData*>(msg.dataPayload);
			createEntity(data->entityDef);
		}
		default:
			break;
	}
}
