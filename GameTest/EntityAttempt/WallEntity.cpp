#include "WallEntity.h"
#include "EntityContainer.h"


void WallEntity::initialise(ContentManager& contentManager, TransformManager& transformManager, Vector2 position,
	float rotation){

	int wallTextureId = contentManager.loadTexture("wall", "floortile.png");

	Vector2 spriteScale = Vector2(400.0f, 10.0f);

	SimpleTransform tempTransform = SimpleTransform(position.x, position.y, spriteScale.x, spriteScale.y, rotation);
	SpriteRenderable wallSprite = SpriteRenderable(wallTextureId);

	transform = transformManager.createTransform(tempTransform);
	renderComponent = transformManager.attachRenderComponent(transform, wallSprite);

	//define rigid body

	b2BodyDef groundBodyDef;
	b2PolygonShape groundBox;
	groundBox.SetAsBox(spriteScale.x / SCALE, spriteScale.y / SCALE);

	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundBox;
	groundBodyDef.angle = DegToRad(rotation);
	

	physicsComponent = transformManager.getPhysicsSystem().createComponent(transform, groundBodyDef, groundFixtureDef);

}

void WallEntity::destroy(TransformManager& transformManager) {
	transformManager.destroyRenderComponent(renderComponent);
	transformManager.destroyRigidBody(physicsComponent);
	transformManager.destroyTransform(transform);
}

void WallEntity::update(EntityContainer& entityContainer) {}

void WallEntity::updateDraw() {
}
