#include "World.h"
#include "MessageReceiver.h"
#include "AudioManager.h"

World::~World() {
	deleteAllListeners();
}

/*
 * veeery simple level loader,
 * theres only 3 entities to pick from
 */
void World::loadLevel(const std::wstring& levelFileName) {
	std::wifstream infile(levelFileName.c_str());
	unsigned numEntities = attemptRead<unsigned>(infile);

	for (int i = 0; i < numEntities; ++i){
		unsigned entityTypeId = attemptRead<unsigned>(infile);
		if(entityTypeId >= EntityType::ENTITY_MAX){
			printf("could not load entity, entity with id: %i does not exist", entityTypeId);
			return;
		}
		EntityType entityType = static_cast<EntityType>(entityTypeId);

		float positionX;
		float positionY;
		float rotation = 0.0f;

		switch(entityType) {
			case ENTITY_WALL:
			{
				positionX = attemptRead<float>(infile);
				positionY = attemptRead<float>(infile);
				rotation = attemptRead<float>(infile);
				entityManager.createWall(Vector2(positionX, positionY), rotation);
				break;
			}

			case ENTITY_PLAYER:
			{
				unsigned inputActorId = attemptRead<unsigned>(infile);
				if (inputActorId >= INPUT_ACTOR_MAX) {
					printf("could not load player entity, player actor with id: %i does not exist", inputActorId);
					return;
				}

				InputActors inputActor = static_cast<InputActors>(inputActorId);
				positionX = attemptRead<float>(infile);
				positionY = attemptRead<float>(infile);
				entityManager.createPlayer(inputActor, Vector2(positionX, positionY));
				break;
			}

			case ENTITY_BALL:
			{
				positionX = attemptRead<float>(infile);
				positionY = attemptRead<float>(infile);
				entityManager.createBall(Vector2(positionX, positionY));
				break;
			}
			case ENTITY_DEBRIS: 
			{
				numDebris = attemptRead<int>(infile);
				break;
			}
			default:
				break;
		}
	}
	


}


void World::loadContent(ContentManager& contentManager) {
	dragonTextureId = contentManager.loadTexture("dragon", "dragon.png");
	//could preload content manager content if we had file to read in
	//that mapped file path to texture name
}

void World::initialise(){
	
	registerInput(INPUT_ACTOR_PLAYER1);

	MessageCallback projectileCallback = std::bind(&World::fireProjectile, this, std::placeholders::_1);
	addListener(MESSAGE_FIRE_PROJECTILE, projectileCallback);

	
	loadLevel(L"../../Levels/level1.txt");

	//this is just for fun
	for (int i = 0; i < numDebris; ++i) {
		Vector2 position = Vector2((-800.0f + (i % 120) * 8), (600.0f - ((i / 180)) * 8));
		entityManager.createDebris(position, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}


	//play music on startup
	//use m to toggle off
	MusicMsgData msgData;
	msgData.command = MusicCommand::COMMAND_PLAY;
	msgData.music = MUSIC_LEVEL1;

	Message msg;
	msg.messageType = MESSAGE_AUDIO_MUSIC;
	msg.timeUntillDispatch = 0;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(msgData);

	pushMessage(msg);
}

void World::handleInput(InputActors inputActor, MappedInput* mappedInput){
	switch (inputActor) {
		case INPUT_ACTOR_PLAYER1:
			handleWorldInput(mappedInput);
			break;
		default:
			break;
	}

}

void World::fireProjectile(Message msg) {
	FireProjectileMessageData* data = static_cast<FireProjectileMessageData*>(msg.dataPayload);
	entityManager.createProjectile(data->position, data->direction);
}

void World::update(Camera& camera){
	entityManager.update(transformManager);
}

void World::handleWorldInput(MappedInput* mappedInput){
	int speed = 15.0f;
	float msec = 33;
	if (mappedInput->getState(InputCooked::States::STATE_CAMERA_MOVE_UP)) {
		camera.SetPosition(camera.GetPosition() + Vector3(0.0f, 1.0f, 0.0f) * speed * (msec / 1000));
	}

	if (mappedInput->getState(InputCooked::States::STATE_CAMERA_MOVE_DOWN)) {
		camera.SetPosition(camera.GetPosition() + Vector3(0.0f, -1.0f, 0.0f) * speed * (msec / 1000));
	}
	if (mappedInput->getState(InputCooked::States::STATE_CAMERA_MOVE_LEFT)) {
		camera.SetPosition(camera.GetPosition() + Vector3(-1.0f, 0.0f, 0.0f) * speed * (msec / 1000));
	}
	if (mappedInput->getState(InputCooked::States::STATE_CAMERA_MOVE_RIGHT)) {
		camera.SetPosition(camera.GetPosition() + Vector3(1.0f, 0.0f, 0.0f) * speed * (msec / 1000));
	}

	if(mappedInput->getAction(InputCooked::ACTION_RESET)){
		entityManager.~EntityManager();
	}

	if (mappedInput->getAction(InputCooked::ACTION_TOGGLE_MUSIC)) {
		MusicMsgData msgData;
		msgData.command = MusicCommand::COMMAND_TOGGLE;
		//not actually necessary here, toggle will pause / unpause
		//whatever is currently playing
		msgData.music = MUSIC_LEVEL1;

		Message msg;
		msg.messageType = MESSAGE_AUDIO_MUSIC;
		msg.timeUntillDispatch = 0;
		msg.dataPayload = &msgData;
		msg.dataSize = sizeof(msgData);

		pushMessage(msg);
	}
}
