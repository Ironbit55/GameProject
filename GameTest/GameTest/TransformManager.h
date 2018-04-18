/*
 * The glue tying game objects, physics objects and render objects together
 * each of these require a transform
 * 
 * game object proccesses input to update physics
 * physics informs game object transform
 * renderer uses game object transform to draw
 * 
 * sure they could all share the same one but that would be LAME
 */

#pragma once
#include <vector>
#include "../MemoryManagement/MemoryPool.h"
#include "PhysicsSystem.h"
#include "IrohRenderer/RenderSystem.h"


class TransformManager
{
public:
	TransformManager(RenderSystem& renderSystem, PhysicsSystem& physicsSystem) : 
									renderSystem(renderSystem), physicsSystem(physicsSystem) {};

	~TransformManager();

	//why are we using handles?
	//its simple pointers scare me

	//returns handle
	SimpleTransform* createTransform(SimpleTransform& trasform);
	
	PhysicsComponent* attachRigidBody(SimpleTransform* objectTransform, const b2BodyDef* body, const b2PolygonShape* polygon);
	RenderComponent* attachRenderComponent(SimpleTransform* objectTransform, RenderComponent& renderable);

	

	void destroyTransform(SimpleTransform* transform);

	void destroyRigidBody(PhysicsComponent* rigidBody);

	void destroyRenderComponent(RenderComponent* r);
	
	
	/*bool renderableDisableDraw(int handle);
	bool renderableEnableDraw(int handle);

	b2Body getRigidBody(int handle);
	b2Body removeRigidBody();

	void getSprites();*/

	MemoryPool<SimpleTransform, 4096> transformPool;

	//yes these should  use some custom memory allocation
	//but i don't want to implement these data structures in them
	//maybe one day...
	//std::vector<SimpleRenderable*> renderList;

	//std::map<int, SimpleRenderable*> handleToRenderableMap;
	//std::map<b2Body*, int> rigidBodyToObjectMap;



	//std::vector<SimpleTransform*> renderableList;
	//std::vector<SimpleTransform*> rigidBodyList;


	
private:
	int nextHandle;
	RenderSystem& renderSystem;
	PhysicsSystem& physicsSystem;

};

