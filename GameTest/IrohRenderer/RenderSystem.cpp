#include "RenderSystem.h"




RenderComponent* RenderSystem::createRenderComponent(SimpleTransform* transform, SpriteRenderable& spriteComponent){
	if(transform == nullptr){
		//can't create render component without parent transform
		return nullptr;
	}
	if (numRenderComponents > MAX_SPRITE_COMPONENTS) {
		cout << "exceeded render components pool size";
		return nullptr;
	}
	numRenderComponents++;
	RenderComponent* r = renderComponentPool.newElement(transform, spriteComponent);
	/*RenderComponent* r = renderComponentPool.allocate();
	r->sprite = spriteComponent;*/

	return r;
}

void RenderSystem::deleteRenderComponent(RenderComponent* component){
	numRenderComponents--;
	renderComponentPool.deallocate(component);
}

void RenderSystem::updateTransforms(){
	renderer.clearSprites();
	RenderComponent* freeSlot = renderComponentPool.firstFree();
	for (RenderComponent* r = renderComponentPool.first(); r != nullptr; r = renderComponentPool.next(r, freeSlot)) {
		r->sprite.position = r->transform->position;
		r->sprite.rotation = r->transform->rotation;
		renderer.addSpriteToDrawList(&(r->sprite));
	}
}

void RenderSystem::update(float msec) {
	updateTransforms();
	renderer.UpdateScene(msec);
}

void RenderSystem::renderScene(){
	renderer.RenderScene();
}

//RenderComponent* RenderSystem::createRenderComponent(RenderComponent& component){
//	RenderComponent* c = renderComponentPool.allocate();
//	c = &component;
//}
