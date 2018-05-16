#include "DebrisEntity.h"
#include "EntityManager.h"


DebrisEntity::DebrisEntity()
{
}


DebrisEntity::~DebrisEntity()
{
}

void DebrisEntity::initialise(ContentManager & contentManager, TransformManager & transformManager, Vector2 position, Vector4 colour) {
	int ballTextureId = contentManager.loadTexture("dragon");

	Vector2 spriteScale = Vector2(5.0f, 5.0f);

	SimpleTransform tempTransform = SimpleTransform(position.x, position.y, spriteScale.x, spriteScale.y, 0);
	SpriteRenderable wallSprite = SpriteRenderable(ballTextureId);
	wallSprite.colour = colour;

	transform = transformManager.createTransform(tempTransform);
	renderComponent = transformManager.attachRenderComponent(transform, wallSprite);
	physicsComponent = transformManager.getPhysicsSystem().createComponentBox(transform, spriteScale);
}

void DebrisEntity::destroy(TransformManager & transformManager) {
	transformManager.destroyRenderComponent(renderComponent);
	transformManager.destroyPhysicsComponent(physicsComponent);
	transformManager.destroyTransform(transform);
}

void DebrisEntity::update(float msec, EntityContainer & entityManager) {
}

EntityType DebrisEntity::getEntityType() {
	return ENTITY_DEBRIS;
}
