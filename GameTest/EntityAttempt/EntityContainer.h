/*
 * Author: Edward Curran
 * 
 * The idea is have a pool that can fit any type of entity which inherits from EntityInterface
 * so as long as entity is under the max size it can be place in the pool
 * then as we know all elements in the pool can be cast to EntityInterface pointers
 * we can call virtual EntityInterface functions on all of them
 * this method obviously wastes a fair bit of memory as not all entities will be of max size
 * 
 * it's pretty ugl because of the excessive use of pointers
 * I'm not against scrapping this whole thing
 */

#pragma once
#include "EntityInterface.h"

const int DELETE_LIST_MAX = 40;
const int MAX_NUM_ENTITIES = 2000;
//this only needs to be so big because some entities inherit from MessageReciever which is pretty big
//because it has an std::map
const int ENTITY_SIZE_BYTES = 256;

typedef unsigned char entitysize_t[ENTITY_SIZE_BYTES];


class EntityContainer
{
public:
	EntityContainer() {}
	~EntityContainer();


	void addToDelete(EntityInterface* entity) {
		if (entityDeleteListCount > DELETE_LIST_MAX) {
			printf("can't delete entity, entity delete list is full for this frame");
		}

		entityDeleteListCount[entityDeleteList] = entity;
		entityDeleteListCount++;
	}

	void update(float msec);

	//void loadContent(ContentManager& contentManager);

	//void initialise(TransformManager& transformManager);



protected:

	EntityInterface* entityDeleteList[DELETE_LIST_MAX];
	int entityDeleteListCount = 0;
	//this just holds pointers to entities
	//a pointer to a pointer yeah i dunno man
	//again if this was an id, it would be reasonable
	MemoryPool<entitysize_t, MAX_NUM_ENTITIES * sizeof(entitysize_t)> entityPool;
	int numEntities = 0;

	EntityInterface* addEntity(int size);

	template <typename EntityType>
	EntityType addEntity() {
		EntityInterface* entity = addEntity(sizeof(EntityType));
		if (entity == nullptr) {
			return nullptr;
		}
		EntityType* entityPtr = reinterpret_cast<EntityType*>(entity);
		new(entityPtr) EntityType();
		return entityPtr;
	}

	void deleteEntities(TransformManager& transformManager);

	void destructAllEntities(TransformManager& transformManager);
};

