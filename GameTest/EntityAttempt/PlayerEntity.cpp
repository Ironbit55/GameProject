#include "PlayerEntity.h"
#include "GameTest/AudioManager.h"
#include "GameTest/World.h"


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

	bool success = registerInput(inputActor);

}

void PlayerEntity::destroy(TransformManager& transformManager) {
	transformManager.destroyRenderComponent(renderComponent);
	transformManager.destroyRigidBody(physicsComponent);
	transformManager.destroyTransform(transform);
}

void PlayerEntity::update(EntityContainer& entityManager) {
	SoundEffectMsgData msgData;
	msgData.effect = EFFECT_JUMP;

	Message msg;
	msg.messageType = MESSAGE_AUDIO_EFFECT;
	msg.timeUntillDispatch = 0;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(msgData);

	//MessagingService::instance().pushMessage(msg);
}

void PlayerEntity::handleInput(InputActors inputActor, MappedInput* mappedInput){


	SoundEffectMsgData msgData;
	msgData.effect = EFFECT_JUMP;

	Message msg;
	msg.messageType = MESSAGE_AUDIO_EFFECT;
	msg.timeUntillDispatch = 0;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(msgData);

	

	if(mappedInput->getAction(InputCooked::ACTION_JUMP)){
		//play jump sound effect
		MessagingService::instance().pushMessage(msg);

		//use controller stick to find direction 
		float magnitude = 50.0f;
		float rangeX = 0.0f;
		float rangeY = 0.0f;

		//will stay zero unless range outside deadzone
		mappedInput->getRange(InputCooked::RANGE_CONTROLLER_LEFT_X, rangeX);
		mappedInput->getRange(InputCooked::RANGE_CONTROLLER_LEFT_Y, rangeY);
		
		Vector2 dir = Vector2(rangeX, -rangeY);
		dir.Normalise();

		Vector2 position = PhysicsSystem::scaleB2Vec2(physicsComponent->body->GetPosition());


		FireProjectileMessageData msgProjectileData;
		msgProjectileData.position = Vector2(position.x + (dir.x * 35.0f), position.y + (dir.y * 35.0f));
		msgProjectileData.direction = dir;

		Message msgProjectile;
		msgProjectile.messageType = MESSAGE_FIRE_PROJECTILE;
		msgProjectile.timeUntillDispatch = 0;
		msgProjectile.dataPayload = &msgProjectileData;
		msgProjectile.dataSize = sizeof(msgProjectileData);

		//launch projetile at position with direction
		MessagingService::instance().pushMessage(msgProjectile);

		//physicsComponent->body->ApplyLinearImpulseToCenter(b2Vec2(dir.x * magnitude, dir.y * magnitude), true);

		//physicsComponent->body->ApplyForceToCenter(b2Vec2(0, 10.0f), true);
	}

	if (mappedInput->getState(InputCooked::STATE_PLAYER_MOVE_UP)) {
		physicsComponent->body->ApplyForceToCenter(b2Vec2(0, 10.3f * SCALE), true);
	}

	if (mappedInput->getState(InputCooked::STATE_PLAYER_MOVE_DOWN)) {
		physicsComponent->body->ApplyLinearImpulseToCenter(b2Vec2(0, -0.1f), true);
		
	}

	if (mappedInput->getState(InputCooked::STATE_PLAYER_MOVE_LEFT)) {
		physicsComponent->body->ApplyLinearImpulseToCenter(b2Vec2(-0.1f, 0), true);
	}

	if (mappedInput->getState(InputCooked::STATE_PLAYER_MOVE_RIGHT)) {
		physicsComponent->body->ApplyLinearImpulseToCenter(b2Vec2(0.1f, 0), true);
	}
}
