#pragma once

#include "EntityInterface.h"
const int DELETE_LIST_MAX = 40;
const int MAX_NUM_ENTITIES = 1000;
class EntityManager
{
public:
	EntityManager();
	~EntityManager();


protected:

	int entitiyDeleteList[DELETE_LIST_MAX];
	//this just holds pointers to entities
	MemoryPool<EntityInterface*, MAX_NUM_ENTITIES * sizeof(EntityInterface*)> entityPool;

};

