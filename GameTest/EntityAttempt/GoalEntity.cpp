#include "GoalEntity.h"
#include "EntityManager.h"
#include "GameRefereeEntity.h"
#include "GameTest/AudioManager.h"


GoalEntity::GoalEntity()
{
}


GoalEntity::~GoalEntity()
{
}

void GoalEntity::initialise(ContentManager& contentManager, TransformManager& transformManager, EntityDef& entityDef) {
	int textureId = contentManager.loadTexture(entityDef.textureName);

	Vector2 spriteScale = entityDef.scale;

	SimpleTransform tempTransform = SimpleTransform(entityDef.position.x, entityDef.position.y, spriteScale.x, spriteScale.y, entityDef.rotation);
	SpriteRenderable wallSprite = SpriteRenderable(textureId);
	wallSprite.colour = Vector4(1.0f, 0, 0, 1);

	playerIndexCanScoreIn = entityDef.inputActor;

	transform = transformManager.createTransform(tempTransform);
	renderComponent = transformManager.attachRenderComponent(transform, wallSprite);

	//define rigid body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	//bodyDef.position = scaleVec2(transform->getOrigin());


	float colliderRadius = 60.0f;

	b2CircleShape circle;
	circle.m_radius = colliderRadius / SCALE;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.isSensor = true;


	b2BodyDef groundBodyDef;
	b2PolygonShape groundBox;
	groundBox.SetAsBox(spriteScale.x / SCALE, spriteScale.y / SCALE);

	createPhysicsComponentFromBody(groundBodyDef, transformManager);
	attachFixture(fixtureDef);
}

void GoalEntity::update(float msec, EntityContainer& entityManager) {
}

EntityType GoalEntity::getEntityType(){
	return ENTITY_GOAL;
}

bool GoalEntity::acceptsContacts() {
	return true;
}

void GoalEntity::onContactBegin(const ContactData& data) {
	if (data.entityCollidingWith == nullptr) {
		return;
	}

	EntityType EntityType = data.entityCollidingWith->getEntityType();
	switch (EntityType)
	{
	case ENTITY_BALL: {
		renderComponent->sprite.colour = Vector4(1, 0, 0, 1);
		EntityBall* ball = static_cast<EntityBall*>(data.entityCollidingWith);
		sendGoalMessage(ball->getLastTouchedPlayer());
		break;
	}
	default:
		break;
	}
}

void GoalEntity::onContactEnd(const ContactData& data) {
}

void GoalEntity::sendGoalMessage(int playerIndex) {
	if(playerIndex != playerIndexCanScoreIn){
		return;
	}

	SoundEffectMsgData msgData;
	msgData.effect = EFFECT_YAY;

	Message msg;
	msg.messageType = MESSAGE_AUDIO_EFFECT;
	msg.timeUntillDispatch = 0;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(msgData);

	MessagingService::instance().pushMessage(msg);

	ScoreGoalMsgData msgDataScore;
	msgDataScore.PlayerIndex = playerIndex;


	Message msgScore;
	msgScore.messageType = MESSAGE_SCORE_GOAL;
	msgScore.timeUntillDispatch = 0;
	msgScore.dataPayload = &msgDataScore;
	msgScore.dataSize = sizeof(msgDataScore);

	MessagingService::instance().pushMessage(msgScore);
}
