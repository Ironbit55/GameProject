#include "TransformManager.h"






TransformManager::~TransformManager()
{
}

SimpleTransform* TransformManager::createTransform(SimpleTransform& transform){
	SimpleTransform* t = transformPool.newElement(transform);
	
	return t;
}

PhysicsComponent* TransformManager::attachPhysicsComponent(SimpleTransform* objectTransform, b2BodyDef& body) {
	PhysicsComponent* p = physicsSystem.createComponent(objectTransform, body);
	p->transform = objectTransform;

	return p;
}

RenderComponent* TransformManager::attachRenderComponent(SimpleTransform* objectTransform, SpriteRenderable& renderable){
	
	/*r.transform = objectTransform;
	r.sprite = renderable;*/
	return renderSystem.createRenderComponent(objectTransform, renderable);
}


void TransformManager::destroyTransform(SimpleTransform* transform){
	if (transform ==  nullptr) {
		return;
	}
	transformPool.deallocate(transform);
}

void TransformManager::destroyPhysicsComponent(PhysicsComponent* p){
	if(p == nullptr){
		return;
	}
	physicsSystem.deleteComponent(p);
}

void TransformManager::destroyRenderComponent(RenderComponent* r){
	if (r == nullptr) {
		return;
	}
	renderSystem.deleteRenderComponent(r);
}

//int TransformManager::createTransform(){
//	SimpleTransform* transform = simpleTransformPool.allocate();
//}
