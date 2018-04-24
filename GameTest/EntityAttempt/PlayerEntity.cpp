#include "PlayerEntity.h"



PlayerEntity::PlayerEntity(){
	
}


PlayerEntity::~PlayerEntity() {
}

void PlayerEntity::initialise(ContentManager& contentManager, TransformManager& transformManager, InputActors inputActor, Vector2 position,
	float rotation) {
	int playerTextureId = contentManager.loadTexture("player", "dragon.png");

	Vector2 spriteScale = Vector2(20.0f, 20.0f);

	SimpleTransform tempTransform = SimpleTransform(position.x, position.y, spriteScale.x, spriteScale.y, rotation);
	SpriteRenderable playerSprite = SpriteRenderable(playerTextureId);


	transform = transformManager.createTransform(tempTransform);
	renderComponent = transformManager.attachRenderComponent(transform, playerSprite);
	physicsComponent = transformManager.getPhysicsSystem().createComponentBox(transform, Vector2(20, 20));

	//bool success = registerInput(inputActor);

}

void PlayerEntity::destroy(TransformManager& transformManager) {
	transformManager.destroyRenderComponent(renderComponent);
	transformManager.destroyRigidBody(physicsComponent);
	transformManager.destroyTransform(transform);
}

void PlayerEntity::update(EntityContainer& entityManager)
{
}

void PlayerEntity::handleInput(InputActors inputActor, MappedInput* mappedInput){
	if(mappedInput->getAction(InputCooked::ACTION_JUMP)){
		physicsComponent->body->ApplyForceToCenter(b2Vec2(0, 1.0f), false);
	}
}
