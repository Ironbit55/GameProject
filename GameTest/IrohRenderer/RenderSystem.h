#pragma once

#include "../../nclgl/Vector4.h"
#include "GameTest/TransformComponents.h"
#include "MemoryManagement/MemoryPool.h"
#include "SpriteRenderer.h"

const int MAX_SPRITE_COMPONENTS = 10000;

class RenderSystem
{
public:
	RenderSystem(SpriteRenderer& renderer) : renderer(renderer) , numRenderComponents(0) {};
	~RenderSystem() {};

	//should return an id really
	RenderComponent* createRenderComponent(SimpleTransform* transform, SpriteRenderable& spriteComponent);

	void deleteRenderComponent(RenderComponent* component);

	//RenderComponent* disableDraw(RenderComponent* component);
	//RenderComponent* enableDraw(RenderComponent* component);

	void updateTransforms();
	
	void update(float msec);

	void renderScene();
protected:
	SpriteRenderer& renderer;

private:
	MemoryPool<RenderComponent, 8192> visibleRenderComponentPool;

	//we set a block size big enough that we don't have to allocate new blocks
	MemoryPool<RenderComponent, MAX_SPRITE_COMPONENTS * sizeof(SpriteRenderable)> renderComponentPool;

	int numRenderComponents;
	
};

