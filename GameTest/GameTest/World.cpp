#include "World.h"
#include "MessageReceiver.h"
#include "AudioManager.h"

World::~World()
{
}

void World::initialise(){
	bool test = registerInput(INPUT_ACTOR_PLAYER1);


	SimpleTransform transform(60.0f, 600.0f, 50.0f, 50.0f, -90.0f);
	SpriteRenderable sprite1 = SpriteRenderable(dragonTextureId, -200.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	/*sprite1.colour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	sprite1.glTexture = dragonTexture;*/
	SimpleTransform* t1 = transformManager.createTransform(transform);
	RenderComponent* r1 = transformManager.attachRenderComponent(t1, sprite1);
	PhysicsComponent* p1 = transformManager.getPhysicsSystem().createComponentBox(t1, Vector2(50, 50));


	transform = SimpleTransform(-300.0f, 0.0f, 20.0f, 20.0f, 0.0f);
	SpriteRenderable sprite2 = SpriteRenderable(raiderTextureId , -200.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	/*sprite2.colour = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	sprite2.glTexture = raiderTexture;*/
	SimpleTransform* t2 = transformManager.createTransform(transform);
	RenderComponent* r2 = transformManager.attachRenderComponent(t2, sprite2);
	PhysicsComponent* p2 = transformManager.getPhysicsSystem().createComponentBox(t2, Vector2(20, 20));

	transform = SimpleTransform(-300.0f, -300.0f, 400.0f, 10.0f, 0.0f);
	SpriteRenderable tileSprite = SpriteRenderable(wallTextureId);
	SimpleTransform* groundT = transformManager.createTransform(transform);
	RenderComponent* groundR = transformManager.attachRenderComponent(groundT, tileSprite);

	b2BodyDef groundBodyDef;
	b2PolygonShape groundBox;
	groundBox.SetAsBox(400.0f / SCALE, 5.0f / SCALE);

	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundBox;
	PhysicsComponent* groundP = transformManager.getPhysicsSystem().createComponent(groundT, groundBodyDef, groundFixtureDef);

	
	
	groundBodyDef.angle = PI / 2;
	
	transform = SimpleTransform(100.0f, -150.0f, 400.0f, 10.0f, 0.0f);
	
	SimpleTransform* wallLeftT = transformManager.createTransform(transform);
	RenderComponent* wallLeftR = transformManager.attachRenderComponent(wallLeftT, tileSprite);
	PhysicsComponent* wallLeftP = transformManager.getPhysicsSystem().createComponent(wallLeftT, groundBodyDef, groundFixtureDef);

	
	
	transform = SimpleTransform(-700.0f, -150.0f, 400.0f, 10.0f, 0.0f);
	SimpleTransform* wallRightT = transformManager.createTransform(transform);
	RenderComponent* wallRightR = transformManager.attachRenderComponent(wallRightT, tileSprite);
	PhysicsComponent* wallRightP = transformManager.getPhysicsSystem().createComponent(wallRightT, groundBodyDef, groundFixtureDef);

	int numSprites = 1000;

	for (int i = 0; i < numSprites; ++i) {
		SimpleTransform transform((-800.0f + (i % 120) * 8), (600.0f - ((i / 180)) * 8), 5.0f, 5.0f, 0.0f);

		SpriteRenderable sprite;
		

		//spritesTemp[i] = SpriteRenderable(Vector3((-960.0f + (i % 180) * 120), (600.0f - ((i / 180)) * 120), -200.0f), Vector3(30.0f, 30.0f, 5.0f));
		if (i % 2 == 0) {
			sprite = SpriteRenderable(dragonTextureId, -100.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		} else {
			sprite = SpriteRenderable(dragonTextureId, -100.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		}

		//spriteListTail++;
		SimpleTransform* transformResult = transformManager.createTransform(transform);
		transformManager.getRenderSystem().createRenderComponent(transformResult, sprite);
		PhysicsComponent* p = transformManager.getPhysicsSystem().createComponentBox(transformResult, Vector2(5, 5));
		//sprites.push_back(&spritesTemp[i]);
	}

	//transformManager.destroyRenderComponent(r2);
	//transformManager.destroyTransform(t2);
	//transformManager.destroyRenderComponent(r1);
	//transformManager.destroyTransform(t1);




	Message msg;
	msg.messageType = MESSAGE_AUDIO_EFFECT;
	msg.timeUntillDispatch = 0;
	SoundEffectMsgData msgData;
	msgData.effect = EFFECT_JUMP;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(msgData);
}

void World::loadContent(ContentManager& contentManager) {
	dragonTextureId = contentManager.loadTexture("dragon.png");
	raiderTextureId = contentManager.loadTexture("raider.png");
	wallTextureId = contentManager.loadTexture("floortile.png");
}

void World::handleInput(InputActors inputActor, MappedInput* mappedInput){
	switch (inputActor) {
		case INPUT_ACTOR_PLAYER1:
			controlCameraInput(mappedInput);
			break;
		default:
			cout << "something else";
			break;
	}

}

void World::controlCameraInput(MappedInput* mappedInput){
	int speed = 15.0f;
	float msec = 33;
	if (mappedInput->getState(InputCooked::States::STATE_MOVE_UP)) {
		cout << "Move Up";
		//audioManager.playSoundEffect(EFFECT_JUMP);
		//MessagingService::instance().pushMessage(msg);
		camera.SetPosition(camera.GetPosition() + Vector3(0.0f, -1.0f, 0.0f) * speed * (msec / 1000));
	}

	if (mappedInput->getState(InputCooked::States::STATE_MOVE_DOWN)) {
		cout << "Move Down";
		camera.SetPosition(camera.GetPosition() + Vector3(0.0f, 1.0f, 0.0f) * speed * (msec / 1000));
	}
	if (mappedInput->getState(InputCooked::States::STATE_MOVE_LEFT)) {
		cout << "Move Left";
		camera.SetPosition(camera.GetPosition() + Vector3(-1.0f, 0.0f, 0.0f) * speed * (msec / 1000));
	}
	if (mappedInput->getState(InputCooked::States::STATE_MOVE_RIGHT)) {
		cout << "Move Right";
		camera.SetPosition(camera.GetPosition() + Vector3(1.0f, 0.0f, 0.0f) * speed * (msec / 1000));
	}
}
