#include "EntityManager.h"
#include "PlayerEntity.h"


EntityInterface* EntityManager::createWall(Vector2 position, float rotation) {
	WallEntity wall = WallEntity();
	wall.initialise(contentManager, transformManager, position, rotation);

	return addEntity(&wall, sizeof(wall));


}

EntityInterface* EntityManager::createPlayer(InputActors inputActor, Vector2 position, float rotation) {
	PlayerEntity player;
	player.initialise(contentManager, transformManager, inputActor, position, rotation);

	EntityInterface* entity =  addEntity(&player, sizeof(player));

	//we have to reigster input once we know the final destination pointer of the entity
	//as the callback needs to be bound to the correct instance
	//this is awkward and this is easiest way to do it...
	//entity->registerInput(inputActor);
	return entity;
}
