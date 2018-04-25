#include "EntityManager.h"
#include "PlayerEntity.h"
#include <new>


EntityManager::~EntityManager() {
	destructAllEntities(transformManager);
}

EntityInterface* EntityManager::createWall(Vector2 position, float rotation) {
	
	//WallEntity wall = WallEntity();
	//wall.initialise(contentManager, transformManager, position, rotation);

	EntityInterface* entity = addEntity(sizeof(WallEntity));
	if(entity == nullptr){
		return nullptr;
	}

	//alternativly construct local Wall instance
	//cast EntityInterface to WallEntity* and assign to &wall
	//this means we need to make sure to implement copy constructors correctly etc

	WallEntity* wallPtr = reinterpret_cast<WallEntity*>(entity);
	
	//alternativly construct local Wall instance
	//cast EntityInterface to WallEntity* and assign to &wall
	//this means we need to make sure to implement copy constructors correctly etc

	//we have the memory location so construct it using placement new

	//i think everything may be out of allignment so everything will probably break horribly at some point
	//the memory pool handles out allignement but only for the object type you initialse it with...
	new(wallPtr) WallEntity();
	//*wallPtr = wall;
	wallPtr->initialise(contentManager, transformManager, position, rotation);
	

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

EntityBall* EntityManager::createBall(Vector2 position) {
	EntityInterface* entity = addEntity(sizeof(PlayerEntity));
	if (entity == nullptr) {
		return nullptr;
	}

	EntityBall* ballPtr = reinterpret_cast<EntityBall*>(entity);
	new(ballPtr) EntityBall();
	ballPtr->initialise(contentManager, transformManager, position);

	return ballPtr;

}

EntityBall* EntityManager::createProjectile(Vector2 position, Vector2 launchDirection) {
	EntityBall* entity = createBall(position);
	if (entity == nullptr) {
		return nullptr;
	}

	entity->launch(launchDirection);
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
