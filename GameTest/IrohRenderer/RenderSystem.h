#pragma once

#include "../../nclgl/Vector4.h"
#include "GameTest/TransformComponents.h"
#include "MemoryManagement/MemoryPool.h"


class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	//should return an id really
	RenderComponent* createRenderComponent(RenderComponent& component);

	void deleteRenderComponent(RenderComponent* component);

	//RenderComponent* disableDraw(RenderComponent* component);
	//RenderComponent* enableDraw(RenderComponent* component);

	void updateTransforms();
	RenderComponent test;


protected:

private:
	MemoryPool<RenderComponent, 4096> visibleRenderComponentPool;
	MemoryPool<RenderComponent, 4096> renderComponentPool;
};

