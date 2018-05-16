#pragma once

#include "GameTest/InputReceiver.h"
#include "PhysicsEntity.h"

class WallEntity : public PhysicsEntity{
public:
	WallEntity() {};
	~WallEntity() {};

	//void loadContent(ContentManager contentManager) override;

	void initialise(ContentManager& contentManager, TransformManager &transformManager, EntityDef entityDef);


	void update(float msec, EntityContainer& entityManager) override;

	void updateDraw() override;

	EntityType getEntityType() override;


	bool acceptsContacts() override;
	void onContactBegin(const ContactData& data) override;
	void onContactEnd(const ContactData& data) override;
private:



};

