#include "RenderSystem.h"




RenderComponent* RenderSystem::createRenderComponent(RenderComponent& component){
	RenderComponent* r = renderComponentPool.allocate();
	r = &component;
	return r;
}

void RenderSystem::deleteRenderComponent(RenderComponent* component){
	renderComponentPool.deallocate(component);
}

void RenderSystem::updateTransforms(){
	RenderComponent* freeSlot = renderComponentPool.firstFree();
	for (RenderComponent* r = renderComponentPool.first(); r != nullptr; r = renderComponentPool.next(r, freeSlot)) {
		r->sprite.position = r->transform->position;
	}
}

//RenderComponent* RenderSystem::createRenderComponent(RenderComponent& component){
//	RenderComponent* c = renderComponentPool.allocate();
//	c = &component;
//}
