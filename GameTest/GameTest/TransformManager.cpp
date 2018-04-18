#include "TransformManager.h"






TransformManager::~TransformManager()
{
}

SimpleTransform* TransformManager::createTransform(SimpleTransform& transform){
	SimpleTransform* t = transformPool.newElement(transform);
	
	return t;
}

PhysicsComponent* TransformManager::attachRigidBody(SimpleTransform* objectTransform, b2BodyDef& body, b2FixtureDef& fixtureDef){
	PhysicsComponent* p = physicsSystem.createComponent(objectTransform, body, fixtureDef);
	p->transform = objectTransform;

	return p;

}

RenderComponent* TransformManager::attachRenderComponent(SimpleTransform* objectTransform, SpriteRenderable& renderable){
	
	/*r.transform = objectTransform;
	r.sprite = renderable;*/
	return renderSystem.createRenderComponent(objectTransform, renderable);
}


void TransformManager::destroyTransform(SimpleTransform* transform){
	transformPool.deallocate(transform);
}

void TransformManager::destroyRigidBody(PhysicsComponent* p){
	physicsSystem.deleteComponent(p);
}

void TransformManager::destroyRenderComponent(RenderComponent* r){
	renderSystem.deleteRenderComponent(r);
}

//int TransformManager::createTransform(){
//	SimpleTransform* transform = simpleTransformPool.allocate();
//}
