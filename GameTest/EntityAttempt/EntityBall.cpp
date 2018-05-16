#include "EntityBall.h"
#include "EntityManager.h"
#include "GoalEntity.h"


EntityBall::EntityBall()  : toDelete(false) {
}


EntityBall::~EntityBall()
{
}

void EntityBall::initialise(ContentManager& contentManager, TransformManager& transformManager, EntityDef entityDef) {

	int ballTextureId = contentManager.loadTexture("ball");

	Vector2 spriteScale = Vector2(8.0f, 8.0f);

	originalPlayerOwner = entityDef.inputActor;
	lastTouchedByPlayer = originalPlayerOwner;

	SimpleTransform tempTransform = SimpleTransform(entityDef.position.x, entityDef.position.y, spriteScale.x, spriteScale.y, 0);
	SpriteRenderable sprite = SpriteRenderable(ballTextureId);
	if (originalPlayerOwner == 0) {
		sprite.colour = Vector4(1, 0, 0, 1);
	} else if (originalPlayerOwner == 1) {
		sprite.colour = Vector4(0, 1, 0, 1);
	} else {
		sprite.colour = entityDef.colour;
	}

	transform = transformManager.createTransform(tempTransform);
	renderComponent = transformManager.attachRenderComponent(transform, sprite);
	createPhysicsComponentCircle(6.0f, transformManager);
	//physicsComponent = transformManager.getPhysicsSystem().createComponentCircle(transform, 9.0f);
}

void EntityBall::launch(Vector2 direction, float magnitudeScale) {
	direction.Normalise();
	float magnitiude = 5.0f + magnitudeScale;
	physicsComponent->body->ApplyLinearImpulseToCenter(b2Vec2(magnitiude * direction.x, magnitiude * direction.y), false);
}

void EntityBall::launch(Vector2 position, Vector2 direction, float magnitudeScale) {
	physicsComponent->body->SetTransform(PhysicsSystem::scaleVec2(position), 0.0f);
	launch(direction, magnitudeScale);
}

void EntityBall::update(float msec, EntityContainer& entityContainer) {
	if(toDelete){
		entityContainer.addToDelete(this);
		//spawnBall();
	}
	Vector2 position = PhysicsSystem::scaleB2Vec2(physicsComponent->body->GetPosition());
	if(position.y < -1000){
		doReset(2000.0f);
	}
}

EntityType EntityBall::getEntityType() {
	return ENTITY_BALL;
}

bool EntityBall::acceptsContacts() {
	return true;
}

void EntityBall::onContactBegin(const ContactData& data) {
	
	if (data.entityCollidingWith == nullptr) {
		return;
	}

	EntityType EntityBType = data.entityCollidingWith->getEntityType();

	switch (EntityBType)
	{
	case ENTITY_WALL: {
		//renderComponent->sprite.colour = Vector4(0, 1, 0, 1);
		break;
	}
	case ENTITY_GOAL: {
		//renderComponent->sprite.colour = Vector4(0, 1, 0, 1);
		GoalEntity* goal = static_cast<GoalEntity*>(data.entityCollidingWith);

		if (goal->getPlayer() == lastTouchedByPlayer) {
			doReset();
		}
		break;
	}
	case ENTITY_PLAYER: {
		toDelete = true;
		break;
	}
	default:
		break;
	}
}

void EntityBall::onContactEnd(const ContactData& data)
{
}

void EntityBall::spawnBall(float delay) {
	RepsawnBallMsgData msgData;
	msgData.owningPlayer = originalPlayerOwner;

	/*CreateEntityMsgData msgData;
	msgData.entityDef = EntityDef();
	msgData.entityDef.entityType = ENTITY_BALL;
	msgData.entityDef.position = defaultPosition;
	msgData.entityDef.inputActor = InputManager::asInputActor(originalPlayerOwner);*/
	

	Message msg;
	msg.messageType = MESSAGE_RESPAWN_BALL;
	msg.timeUntillDispatch = delay;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(msgData);
	MessagingService::instance().pushMessage(msg);
}
