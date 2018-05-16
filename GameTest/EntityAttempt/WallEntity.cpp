#include "WallEntity.h"
#include "EntityManager.h"


void WallEntity::initialise(ContentManager& contentManager, TransformManager& transformManager, EntityDef entityDef){

	int wallTextureId = contentManager.loadTexture("wall");

	Vector2 spriteScale = entityDef.scale;

	SimpleTransform tempTransform = SimpleTransform(entityDef.position.x, entityDef.position.y, spriteScale.x, spriteScale.y, entityDef.rotation);
	SpriteRenderable wallSprite = SpriteRenderable(wallTextureId);

	transform = transformManager.createTransform(tempTransform);
	renderComponent = transformManager.attachRenderComponent(transform, wallSprite);

	//define rigid body

	b2BodyDef groundBodyDef;
	b2PolygonShape groundBox;
	groundBox.SetAsBox(spriteScale.x / SCALE, spriteScale.y / SCALE);

	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundBox;
	groundBodyDef.angle = DegToRad(entityDef.rotation);
	
	createPhysicsComponentFromBody(groundBodyDef, transformManager);
	attachFixture(groundFixtureDef);
	//physicsComponent = transformManager.getPhysicsSystem().createComponent(transform, groundBodyDef);
	//physicsComponent->body->CreateFixture(&groundFixtureDef);

}


void WallEntity::update(float msec, EntityContainer& entityContainer) {}

void WallEntity::updateDraw() {
}

EntityType WallEntity::getEntityType() {
	return ENTITY_WALL;
}

bool WallEntity::acceptsContacts() {
	return true;
}

void WallEntity::onContactBegin(const ContactData& data) {
	if(data.entityCollidingWith == nullptr){
		return;
	}

	EntityType EntityBType = data.entityCollidingWith->getEntityType();
	switch(EntityBType)
	{
	case ENTITY_BALL: {
		renderComponent->sprite.colour = Vector4(1, 0, 0, 1);
		break;
	}
	default:
			break;
	}
}

void WallEntity::onContactEnd(const ContactData& data)
{
}
