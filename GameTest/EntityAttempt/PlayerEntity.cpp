#include "PlayerEntity.h"
#include "GameTest/AudioManager.h"
#include "GameTest/World.h"
#include "../sdlgl/SdlInput.h"

PlayerEntity::PlayerEntity(){
	
}


PlayerEntity::~PlayerEntity() {
}

void PlayerEntity::initialise(ContentManager& contentManager, TransformManager& transformManager, InputActors inputActor, Vector2 position,
	float rotation) {
	int playerTextureId = contentManager.loadTexture("dragon");

	Vector2 spriteScale = Vector2(12.0f, 18.0f);

	SimpleTransform tempTransform = SimpleTransform(position.x, position.y, spriteScale.x, spriteScale.y, rotation);
	SpriteRenderable playerSprite = SpriteRenderable(playerTextureId);


	transform = transformManager.createTransform(tempTransform);
	renderComponent = transformManager.attachRenderComponent(transform, playerSprite);
	//physicsComponent = transformManager.getPhysicsSystem().createComponentBox(transform, Vector2(20, 20));
	createPhysicsComponentBox(spriteScale, transformManager);

	owningPlayer = inputActor;
	bool success = registerInput(inputActor);

}

void PlayerEntity::destroy(TransformManager& transformManager) {
	transformManager.destroyRenderComponent(renderComponent);
	transformManager.destroyPhysicsComponent(physicsComponent);
	transformManager.destroyTransform(transform);
}

void PlayerEntity::update(float msec, EntityContainer& entityManager) {
	if(onGround){
		jumpCharges = 2;
	}
	if(numBalls > 0) {
		renderComponent->sprite.colour = Vector4(1, 0, 0, 1);
	}else {
		renderComponent->sprite.colour = Vector4(1, 1, 1, 1);
	}
}

void PlayerEntity::handleInput(InputActors inputActor, MappedInput* mappedInput){


	SoundEffectMsgData msgData;
	msgData.effect = EFFECT_JUMP;

	Message msg;
	msg.messageType = MESSAGE_AUDIO_EFFECT;
	msg.timeUntillDispatch = 0;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(msgData);

	

	if(mappedInput->getAction(InputCooked::ACTION_JUMP) && numBalls > 0){
		
		//play jump sound effect
		MessagingService::instance().pushMessage(msg);

		//send controller rumble message
		ControllerRumbleMsgData rumbleMsgData;
		rumbleMsgData.inputActor = inputActor;
		
		Message msgRumble;
		msgRumble.messageType = MESSAGE_CONTROLLER_RUMBLE;
		msgRumble.timeUntillDispatch = 0;
		msgRumble.dataPayload = &rumbleMsgData;
		msgRumble.dataSize = sizeof(rumbleMsgData);

		pushMessage(msgRumble);



		//use controller stick to find direction 
		float rangeX = 0.0f;
		float rangeY = 0.0f;
		Vector2 dir;

		//will stay zero unless range outside deadzone
		bool deadzoneX = !mappedInput->getRange(InputCooked::RANGE_CONTROLLER_LEFT_X, rangeX);
		bool deadzoneY = !mappedInput->getRange(InputCooked::RANGE_CONTROLLER_LEFT_Y, rangeY);
		if(deadzoneX && deadzoneY){
			dir = Vector2(0, 1.0f);
		} else {
			dir = Vector2(rangeX, -rangeY);
		}
		dir.Normalise();

		//FIRE!
		shootBall(dir);
	}
	if (mappedInput->getAction(InputCooked::ACTION_CONTROLLER_X)) {
		if (jumpCharges > 0) {
			jumpCharges--;
			physicsComponent->body->ApplyLinearImpulseToCenter(b2Vec2(0, 0.6f * SCALE), true);
		}
	}
	//if (mappedInput->getState(InputCooked::STATE_PLAYER_MOVE_UP)) {
	//	if (jumpCharges > 0) {
	//		jumpCharges--;
	//		physicsComponent->body->ApplyForceToCenter(b2Vec2(0, 40.0f * SCALE), true);
	//	}
	//}

	if (mappedInput->getState(InputCooked::STATE_PLAYER_MOVE_DOWN)) {
		physicsComponent->body->ApplyLinearImpulseToCenter(b2Vec2(0, -0.1f), true);
		
	}

	float magnitude = -0.02f;

	if (mappedInput->getState(InputCooked::STATE_PLAYER_MOVE_LEFT)) {
		if(onGround){
			magnitude = -0.03f;
		}else{
			magnitude = -0.006f;
		}
		physicsComponent->body->ApplyLinearImpulseToCenter(b2Vec2(magnitude, 0), true);
	}

	if (mappedInput->getState(InputCooked::STATE_PLAYER_MOVE_RIGHT)) {
		if (onGround) {
			magnitude = 0.03f;
		} else {
			magnitude = 0.006f;
		}
		physicsComponent->body->ApplyLinearImpulseToCenter(b2Vec2(magnitude, 0), true);
	}
}

void PlayerEntity::shootBall(Vector2 dir) {
	if(numBalls < 1){
		return;
	}



	Vector2 position = PhysicsSystem::scaleB2Vec2(physicsComponent->body->GetPosition());


	FireProjectileMessageData msgProjectileData;
	float offsetRadius = 32.0f;
	Vector2 offset = dir * offsetRadius;
	msgProjectileData.position = position + offset;
	msgProjectileData.direction = dir;
	msgProjectileData.lastTouchedByPlayer = owningPlayer;
	msgProjectileData.originalOwnerPlayer = InputManager::asInputActor(ballOwners[numBalls - 1]);

	float scale = 0.004f;
	Vector2 velocity = PhysicsSystem::scaleB2Vec2(physicsComponent->body->GetLinearVelocity());
	float magnitiude = (scale * velocity.Length());
	msgProjectileData.magnitude = magnitiude;

	Message msgProjectile;
	msgProjectile.messageType = MESSAGE_FIRE_PROJECTILE;
	msgProjectile.timeUntillDispatch = 0;
	msgProjectile.dataPayload = &msgProjectileData;
	msgProjectile.dataSize = sizeof(msgProjectileData);

	//launch projetile at position with direction
	MessagingService::instance().pushMessage(msgProjectile);

	numBalls--;
}


EntityType PlayerEntity::getEntityType() {
	return ENTITY_PLAYER;
}

bool PlayerEntity::acceptsContacts() {
	return true;
}

void PlayerEntity::onContactBegin(const ContactData& data) {
	if (data.entityCollidingWith == nullptr) {
		return;
	}

	EntityType EntityBType = data.entityCollidingWith->getEntityType();
	switch (EntityBType)
	{
	case ENTITY_BALL: {
		data.b2Contact->SetEnabled(false);
		EntityBall* ball = static_cast<EntityBall*>(data.entityCollidingWith);
		ballOwners[numBalls] = ball->getOriginalOwner();
		numBalls++;
		break;
	}
	case ENTITY_WALL: {
		onGround = true;
	}
	default:
		break;
	}
}

void PlayerEntity::onContactEnd(const ContactData& data){
	if (data.entityCollidingWith == nullptr) {
		return;
	}

	EntityType EntityBType = data.entityCollidingWith->getEntityType();
	switch (EntityBType)
	{
	case ENTITY_WALL: {
		onGround = false;
	}
	default:
		break;
	}
}

void PlayerEntity::onContactPreSolve(const ContactData& data) {
	if (data.entityCollidingWith == nullptr) {
		return;
	}

	EntityType EntityBType = data.entityCollidingWith->getEntityType();
	switch (EntityBType)
	{
	case ENTITY_BALL: {
		break;
	}
	default:
		break;
	}
}
