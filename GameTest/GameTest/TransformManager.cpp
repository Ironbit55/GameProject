#include "TransformManager.h"






TransformManager::~TransformManager()
{
}

SimpleTransform* TransformManager::createTransform(SimpleTransform& transform){
	SimpleTransform* t = transformPool.allocate();
	t = &transform;
	return t;
}

PhysicsComponent* TransformManager::attachRigidBody(SimpleTransform* objectTransform, const b2BodyDef* body, const b2PolygonShape* polygon){
	PhysicsComponent* p = physicsSystem.createComponent(body, polygon);
	p->transform = objectTransform;

	return p;

}

RenderComponent* TransformManager::attachRenderComponent(SimpleTransform* objectTransform, RenderComponent& renderable){
	renderable.transform = objectTransform;
	return renderSystem.createRenderComponent(renderable);
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
